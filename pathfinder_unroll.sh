PROJECT_DIR=/localhome/mha157/new_salam/gem5-SALAM
BM_DIR=/localhome/mha157/new_salam/gem5-SALAM/benchmarks/AD/pathfinder/hw
BENCHMARK=pathfinder
declare -a UNROLL=(4 32 64 128);

for unr in "${UNROLL[@]}"; do
    rm 2.txt
    touch 2.txt
    rm 1.txt
    touch 1.txt
    sed -i '40d' "${BM_DIR}/${BENCHMARK}.c" 
    sed -i '40i	#pragma clang loop unroll_count('"${unr}"')' "${BM_DIR}/${BENCHMARK}.c"
    cd "${BM_DIR}"
    make -j
    cd "${PROJECT_DIR}"
    ./ad.sh -b pathfinder &
    PID="${!}"
    while [[  $(grep -c "Performance Analysis" 2.txt) -le 1 ]] && [[ $(grep -c "panic" 1.txt)  -eq 0 ]]
    do
        echo "Pathfinder not finished!"
        sleep 10
    done
    # kill -9 "${PID}"
    echo "Pathfinder finished with bin size: ${unr}!"
    python3 layer_stat_extractor.py "${unr}" >> layer_result.csv
done