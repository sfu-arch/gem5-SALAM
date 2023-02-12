PROJECT_DIR=/localhome/mha157/new_salam/gem5-SALAM
declare -a BIN_SIZES=(4 8 16 32 64 128);
declare -a benchmarks=("pathfinder" "somier");

for benchmark in ${benchmarks[@]}; do

    BM_DIR="/localhome/mha157/new_salam/gem5-SALAM/benchmarks/AD/${benchmark}/hw"
    for bin_size in ${BIN_SIZES[@]}; do
        rm 2.txt
        touch 2.txt
        rm 1.txt
        touch 1.txt
        cd "${BM_DIR}"
        make BIN_SIZE="${bin_size}" -j
        cd "${PROJECT_DIR}"
        ./ad.sh -b pathfinder &
        PID="${!}"
        while [[  $(grep -c "Performance Analysis" 2.txt) -le 1 ]] && [[ $(grep -c "panic" 1.txt)  -eq 0 ]]
        do
            echo "Pathfinder not finished!"
            sleep 10
        done
        kill -9 "${PID}"
        echo "Pathfinder finished with bin size: ${bin_size}!"
        python3 layer_stat_extractor.py "${bin_size}" >> layer_result.csv
    done
done