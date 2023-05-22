#!/bin/bash

# Define colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No color
BULLET='\xE2\x80\xA2' # Unicode character for bullet point

usage="Usage: $0 -b [BENCHMARK] -s [SPAD_SIZE(bytes)] -c [CACHE_SIZE(bytes)]
where BENCHMARK is the name of the benchmark from the benchmarks/AD directory."

FLAGS=""

BENCH=""
DEBUG="false"
PRINT_TO_FILE="true"
VALGRIND="false"
# CACHE_SIZES=(1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152)
CACHE_SIZE=16384
SPADSize=512
declare -a MODES=("tf" "orig")

BIN_CONFIG_PATH="/localhome/mha157/new_salam/gem5-SALAM/src/hwacc/bin_config.txt"

while getopts ":b:s:c:vdp" opt; do
	case $opt in
		b )
			BENCH=${OPTARG}
			;;
		d )
			DEBUG="true"
			;;
		p )
			PRINT_TO_FILE="true"
			;;
		f )
			if [ -z "${FLAGS}" ]
			then
				FLAGS+="${OPTARG}"
			else
				FLAGS+=",${OPTARG}"
			fi
			;;
		v )
			VALGRIND="true"
			;;
		s )
			SPAD_SIZE=${OPTARG}			
			;;
		c ) 
			CACHE_SIZE=${OPTARG}
			;;
		* )
			echo "Invalid argument: ${OPTARG}"
			printf "$usage\n"
			exit 1
			;;
	esac
done

if [ "${BENCH}" == "" ]; then
	echo "No benchmark specified."
	printf "$usage\n"
	exit 2
fi

python3 ad_benchmark_verifier.py "benchmarks/AD/${BENCH}"
if [ $? -ne 0 ]; then
	echo -e "${RED} ${BULLET} Python validator failed. Exiting.${NC}"
    exit 1
else
	echo -e "${GREEN} ${BULLET} Python validator passed.${NC}"
fi

cd $M5_PATH/benchmarks/AD/$BENCH/hw && make -j
if [ $? -ne 0 ]; then
	echo -e "${RED} ${BULLET} Makefile failed. Exiting.${NC}"
    exit 1
else
	echo -e "${GREEN} ${BULLET} Makefile passed.${NC}"
fi
cd .. && make -j
cd $M5_PATH
${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/AD/${BENCH}"

OUTDIR=$M5_PATH/BM_ARM_OUT/AD/$BENCH

if [ "${DEBUG}" == "true" ]; then
	BINARY="ddd --gdb --args ${M5_PATH}/build/ARM/gem5.debug"
elif [ "${VALGRIND}" == "true" ]; then
	BINARY="valgrind \
	--leak-check=yes \
	--suppressions=util/valgrind-suppressions \
	--suppressions=util/salam.supp \
	--track-origins=yes \
	--error-limit=no \
	--leak-check=full \
	--show-leak-kinds=definite,possible \
	--show-reachable=no \
	--log-file=${OUTDIR}/valgrind.log \
	${M5_PATH}/build/ARM/gem5.debug"
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

KERNEL=$M5_PATH/benchmarks/AD/$BENCH/sw/main.elf

SYS_OPTS="--mem-size=4GB \
		  --mem-type=DDR4_2400_8x8 \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=O3CPU"
CACHE_OPTS="--caches --acc_cache"

DEBUG_FLAGS=""

if [ "${FLAGS}" != "" ]; then
	DEBUG_FLAGS+="--debug-flags="
	DEBUG_FLAGS+=$FLAGS
fi

RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
			configs/SALAM/generated/fs_$BENCH.py $SYS_OPTS \
			--accpath=$M5_PATH/benchmarks/AD \
			--accbench=$BENCH $CACHE_OPTS"


PY_FILE_PATH="/localhome/mha157/new_salam/gem5-SALAM/configs/SALAM/generated/$BENCH.py"

for mode in ${MODES[@]}; do
	sed -i '27d' $PY_FILE_PATH
	sed -i '27i\	clstr._connect_caches(system, options, l2coherent=False, cache_size="'${CACHE_SIZE}'B")' $PY_FILE_PATH
	sed -i '1d' $BIN_CONFIG_PATH
	if [ $mode == "tf" ]; then
		LINE="1,512,"
	else
		LINE="0,"
	fi
	> $BIN_CONFIG_PATH
	echo -e $LINE >> $BIN_CONFIG_PATH
	if [ "${PRINT_TO_FILE}" == "true" ]; then
		mkdir -p $OUTDIR
		$RUN_SCRIPT > ${OUTDIR}/debug-trace.txt > 2.txt 2> 1.txt &
	else
		$RUN_SCRIPT > 2.txt 2> 1.txt & 
	fi
	PID="${!}"
	while [[  $(grep -c "Performance Analysis" 2.txt) -le 1 ]] && [[ $(grep -c "END LIBC BACKTRACE" 1.txt)  -eq 0 ]]
	do
		echo "${BENCH} not finished!"
		sleep 10
	done
	echo "Done!"
	cp 2.txt $OUTDIR/SALAM_OUT_${mode}_${CACHE_SIZE}.txt
	cp 1.txt $OUTDIR/log_${mode}_${CACHE_SIZE}.txt
	python3 cache_stat_extractor.py ${BENCH}_${mode} ${CACHE_SIZE}  >> results.csv
	rm $OUTDIR/SALAM_OUT_${mode}_${CACHE_SIZE}.txt
	rm $OUTDIR/log_${mode}_${CACHE_SIZE}.txt
	kill -9 $PID
done
