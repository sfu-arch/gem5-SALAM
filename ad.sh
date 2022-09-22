#!/bin/bash
FLAGS=""

BENCH=""
DEBUG="false"
PRINT_TO_FILE="true"
VALGRIND="false"
# CACHE_SIZES=(1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152)
CACHE_SIZES=(512)

BIN_SCALES=(16 16 8 4 2)
# MODES=("ad" "orig")
MODES=("ad")

BIN_CONFIG_PATH="/localhome/mha157/new_salam/gem5-SALAM/src/hwacc/bin_config.txt"
# PY_FILE_PATH="/localhome/mha157/new_salam/gem5-SALAM/1.txt"

while getopts ":b:f:vdp" opt; do
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
		* )
			echo "Invalid argument: ${OPTARG}"
			echo "Usage: $0 -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
			exit 1
			;;
	esac
done

if [ "${BENCH}" == "" ]; then
	echo "No benchmark specified."
	echo "Usage: $0 -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
	exit 2
fi

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

# ${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName $BENCH --benchDir "benchmarks/AD/${BENCH}"


PY_FILE_PATH="/localhome/mha157/new_salam/gem5-SALAM/configs/SALAM/generated/$BENCH.py"

for mode in ${MODES[@]}; do
	for t in ${CACHE_SIZES[@]}; do
		sed -i '27d' $PY_FILE_PATH
		sed -i '27i\	clstr._connect_caches(system, options, l2coherent=False, cache_size="'${t}'B")' $PY_FILE_PATH
		sed -i '1d' $BIN_CONFIG_PATH
		if [ $mode == "ad" ]; then
			LINE="1,512,"
		else
			LINE="0,"
		fi
		# for u in ${BIN_SCALES[@]}; do
		# 	DIV=$((t/u))
		# 	LINE="${LINE}${DIV},"
		# done
		echo $LINE >> $BIN_CONFIG_PATH
		if [ "${PRINT_TO_FILE}" == "true" ]; then
			mkdir -p $OUTDIR
			$RUN_SCRIPT > ${OUTDIR}/debug-trace.txt > 2.txt 2> 1.txt
		else
			$RUN_SCRIPT  > 2.txt 2> 1.txt
		fi
		echo "Done with cache size: ${t}"
		cp 2.txt $OUTDIR/SALAM_OUT_${mode}_${t}.txt
		cp 1.txt $OUTDIR/log_${mode}_${t}.txt
		# python3 cache_stat_extractor.py ${BENCH}_${mode} $t  >> cache_result.csv
		rm $OUTDIR/SALAM_OUT_${mode}_${t}.txt
		rm $OUTDIR/log_${mode}_${t}.txt
	done
done
# Debug Flags List
#
# IOAcc
# ClassDetail
# CommInterface
# ComputeUnit
# LLVMInterface
# ComputeNode
# LLVMRegister
# LLVMOp
# LLVMParse
# LLVMGEP
# LLVMRuntime == ComputeNode + LLVMRegister + LLVMOp + LLVMParse
# NoncoherentDma - bfs, fft, gemm, md-knn, nw, spmv


