BENCHMARK=matd
PROJECT_DIR=/localhome/mha157/new_salam/gem5-SALAM
BM_DIR=/localhome/mha157/new_salam/gem5-SALAM/benchmarks/AD/"${BENCHMARK}"/hw

# somier = (8, 13, 22)
# pathfinder = (4, 8, 32)
# mttkrp = (6, 8, 16)
# mass_spring = (24 32 64)
# gravity = (10, 15, 24)
# nn = (10 20 40)
# logsum = (1000 2000 10000)
# matd = (24 40 80)

declare -a WORKING_SET_SIZES=(24 40 8);

for set_size in "${WORKING_SET_SIZES[@]}"; do
    rm 2.txt
    touch 2.txt
    rm 1.txt
    touch 1.txt
    cd "${BM_DIR}"
    make  N="${set_size}" -j
    cd "${PROJECT_DIR}"
    ./ad.sh -b "${BENCHMARK}"
    python3 layer_stat_extractor.py "${set_size}" >> "${BENCHMARK}"_layer_result.csv
done

cp cache_result.csv working_set_results/"${BENCHMARK}"_working_set.csv
cp cache_result_template.csv cache_result.csv
