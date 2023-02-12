PROJECT_DIR=/localhome/mha157/new_salam/gem5-SALAM
declare -a BIN_SIZES=(4 16 32 64 128 256);
# declare -a BIN_SIZES=(16 );

# declare -a benchmarks=("pathfinder" "somier" "mass_spring" "gravity" "lenet");
declare -a benchmarks=("mttkrp");

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
        ./ad.sh -b "${benchmark}"
        echo "${benchmark} finished with bin size: ${bin_size}!"
        python3 layer_stat_extractor.py "${bin_size}" >> layer_result.csv
    done
done