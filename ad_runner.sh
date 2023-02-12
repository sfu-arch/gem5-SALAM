#!/bin/bash
# BENCH_NAMES=("nn" "alexnet" "gravity" "logsum" "matd" "pathfinder" "sddmm" "somier" "mass_spring")
BENCH_NAMES=("mass_spring" )

CURR_DIR=$(pwd)
MAKE_DIR="/localhome/mha157/new_salam/gem5-SALAM/benchmarks/AD/"
for BENCH in ${BENCH_NAMES[@]}; do
    cd $MAKE_DIR$BENCH
    make -j;
    cd $CURR_DIR
    cp cache_result_template.csv cache_result.csv
    ./ad.sh -b $BENCH
    cp cache_result.csv ${BENCH}_cache_result.csv
    cp cache_result_template.csv cache_result.csv
done