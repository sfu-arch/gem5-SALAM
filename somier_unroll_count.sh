BENCHMARK=somier
PROJECT_DIR=/localhome/mha157/new_salam/gem5-SALAM
BM_DIR=/localhome/mha157/new_salam/gem5-SALAM/benchmarks/AD/"${BENCHMARK}"/hw

declare -a UNROLL_PARAMS=(1 8 16);

for unroll_count in "${UNROLL_PARAMS[@]}"; do
    rm 2.txt
    touch 2.txt
    rm 1.txt
    touch 1.txt
    cd "${BM_DIR}"
    make UNROLL_COUNT="${unroll_count}" 
    cd "${PROJECT_DIR}"
    ./ad.sh -b somier &
    PID="${!}"
    while [[  $(grep -c "Performance Analysis" 2.txt) -le 1 ]] && [[ $(grep -c "panic" 1.txt)  -eq 0 ]]
    do
        echo "somier not finished!"
        sleep 10
    done
    # sleep 10
    # kill -9 "${PID}"
    echo "Somier finished with bin size: ${unroll_count}!"
    python3 layer_stat_extractor.py "${unroll_count}" >> somier_layer_result.csv
done