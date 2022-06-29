#!/bin/bash
FLAGS=""

BENCH=""
DEBUG="false"
PRINT_TO_FILE="false"
VALGRIND="false"
CACHE_SIZES=(512 1024 2048 4096)
BIN_SCALES=(16 16 8 4 2)
BIN_CONFIG_PATH="/localhome/mha157/gem5-SALAM/src/hwacc/bin_config.txt"
PY_FILE_PATH="/localhome/mha157/gem5-SALAM/configs/SALAM/generated/gravity.py"
# PY_FILE_PATH="/localhome/mha157/gem5-SALAM/1.txt"

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
	BINARY="${M5_PATH}/build/ARM/gem5.debug"
fi

KERNEL=$M5_PATH/benchmarks/AD/$BENCH/sw/main.elf

SYS_OPTS="--mem-size=4GB \
		  --mem-type=DDR4_2400_8x8 \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=TimingSimpleCPU"
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


if [ "${PRINT_TO_FILE}" == "true" ]; then
    mkdir -p $OUTDIR
    $RUN_SCRIPT > ${OUTDIR}/debug-trace.txt
else
    $RUN_SCRIPT
fi

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


