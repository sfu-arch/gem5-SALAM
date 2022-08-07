#!/bin/bash
#
#SBATCH --cpus-per-task=8
#SBATCH --time=10:00
#SBATCH --mem=5G
export M5_PATH=/data/gem5-baseline

srun $M5_PATH/build/RISCV/gem5.opt --debug-start=0 --debug-end=60000 --debug-file=trace.out --outdir=Simple_m5out --debug-flags=Event,ExecAll,FmtFlag ./gem5-config/run_micro.py Simple SingleCycle ./benchmarks/hello.riscv.bin
