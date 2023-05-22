from math import ceil
import sys
import csv

dump_dir = '1.txt'
salam_stat_dir = '2.txt'
gem5_stat_dir = 'stats.txt'
cache_result_dir = 'cache_result.csv'

L2_EN = False
ad_mode = False

benchmark_name = sys.argv[1]
cache_size = sys.argv[2]
if 'ad' in benchmark_name:
    ad_mode = True

ad_rev_pure_streaming_accesses = 0
ad_rev_total_accesses = 0
if not ad_mode:
    with open(cache_result_dir, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            if row['benchmark'] == benchmark_name.split('_')[0]+'_ad_pure' and row['cache_size'] == cache_size:
                ad_rev_pure_streaming_accesses = int(row['rev_dram_access'])
            if row['benchmark'] == benchmark_name.split('_')[0]+'_ad_total' and row['cache_size'] == cache_size:
                ad_rev_total_accesses = int(row['rev_dram_access'] )

ad_rev_extra_accesses = ad_rev_total_accesses - ad_rev_pure_streaming_accesses
current_mode = 'F' # start with forward mode (F)

forward_cycle_count = 0
total_cycle_count = 0
reverse_cycle_count = 0

forward_mem_access = 0

forward_cluster_cache_access = 0
forward_cluster_cache_misses = 0
forward_cluster_cache_hits = 0

forward_l2_cache_access = 0
forward_l2_cache_misses = 0
forward_l2_cache_hits = 0

reverse_mem_access = 0

reverse_cluster_cache_access = 0
reverse_cluster_cache_misses = 0
reverse_cluster_cache_hits = 0

reverse_l2_cache_access = 0
reverse_l2_cache_misses = 0
reverse_l2_cache_hits = 0

def handle_mem_access(line, mode):
    global forward_mem_access
    global reverse_mem_access
    global forward_cluster_cache_access
    global reverse_cluster_cache_access
    global forward_l2_cache_access
    global reverse_l2_cache_access
    global forward_cluster_cache_misses
    global reverse_cluster_cache_misses
    global forward_l2_cache_misses
    global reverse_l2_cache_misses
    global forward_cluster_cache_hits
    global reverse_cluster_cache_hits
    global forward_l2_cache_hits
    global reverse_l2_cache_hits

    if mode == 'F':
        if 'cluster_cache' in line:
            if 'MISS' in line:
                forward_cluster_cache_misses += 1
            elif 'HIT' in line:
                forward_cluster_cache_hits += 1
            forward_cluster_cache_access += 1
        elif 'l2' in line:
            if 'MISS' in line:
                forward_l2_cache_misses += 1
            elif 'HIT' in line:
                forward_l2_cache_hits += 1
            forward_l2_cache_access += 1
    if mode == 'R':
        if 'cluster_cache' in line:
            if 'MISS' in line:
                reverse_cluster_cache_misses += 1
            elif 'HIT' in line:
                reverse_cluster_cache_hits += 1
            reverse_cluster_cache_access += 1
        elif 'l2' in line:
            if 'MISS' in line:
                reverse_l2_cache_misses += 1
            elif 'HIT' in line:
                reverse_l2_cache_hits += 1
            reverse_l2_cache_access += 1

read_count = 0

dump_file = open(dump_dir, 'r', encoding="ISO-8859-1")
for line in dump_file:
    if 'Cycle count inv' in line:
        forward_cycle_count = int(line.split('=')[-1])
    if 'Cycle count end' in line:
        total_cycle_count = int(line.split('=')[-1])
    if 'Found invert' in line:
        current_mode = 'R'
    if ': MISS' in line or ': HIT' in line:
        handle_mem_access(line, current_mode)
    if 'is_read' in line:
        read_count += 1
salam_stat_file = open(salam_stat_dir, 'r', encoding="ISO-8859-1")
runtime = 0
ad_pure_dram_reads = 0
for line in salam_stat_file:
    if runtime == 0 and 'Runtime:' in line and 'us' in line:
        runtime = float(line.split(':')[-1].split('us')[0])
    if ad_pure_dram_reads == 0 and 'Bins Total DRAM Reads:' in line:
        ad_pure_dram_reads = int(line.split(':')[-1])

ad_pure_dram_read_reqs = ceil(ad_pure_dram_reads / (8))

reverse_cycle_count = total_cycle_count - forward_cycle_count

forward_runtime = forward_cycle_count/total_cycle_count * runtime
reverse_runtime = reverse_cycle_count/total_cycle_count * runtime

forward_mem_access = (forward_l2_cache_misses if L2_EN else forward_cluster_cache_misses)
reverse_mem_access = (reverse_l2_cache_misses if L2_EN else reverse_cluster_cache_misses) 
fw_mem_bytes = forward_mem_access * 8 * 8
rev_mem_bytes = reverse_mem_access * 8 * 8

if 'ad' in benchmark_name:
    rev_mem_bytes += ad_pure_dram_read_reqs*8*8
    print("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}".format(benchmark_name, cache_size,
            forward_cluster_cache_hits, forward_cluster_cache_misses, forward_cluster_cache_access,
            reverse_cluster_cache_hits, reverse_cluster_cache_misses, reverse_cluster_cache_access,
            forward_cluster_cache_misses, reverse_cluster_cache_misses+ad_pure_dram_read_reqs, forward_cluster_cache_misses + reverse_cluster_cache_misses + ad_pure_dram_read_reqs,
            forward_cycle_count, reverse_cycle_count, total_cycle_count,\
            round(fw_mem_bytes/forward_runtime, 1), round(rev_mem_bytes/reverse_runtime, 1), round((fw_mem_bytes + rev_mem_bytes)/runtime, 1),\
            read_count, round(read_count/runtime, 1)))
else:
    rev_total_dram_access = max(reverse_cluster_cache_misses - ad_rev_extra_accesses, 0)
    print("{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{},{}".format(benchmark_name, cache_size,
            forward_cluster_cache_hits, forward_cluster_cache_misses, forward_cluster_cache_access,
            reverse_cluster_cache_hits, reverse_cluster_cache_misses, reverse_cluster_cache_access,
            forward_cluster_cache_misses, reverse_cluster_cache_misses, forward_cluster_cache_misses + reverse_cluster_cache_misses,
            forward_cycle_count, reverse_cycle_count, total_cycle_count,\
            round(fw_mem_bytes/forward_runtime, 1), round(rev_mem_bytes/reverse_runtime, 1), round((fw_mem_bytes + rev_mem_bytes)/runtime, 1),\
            read_count, round(read_count/runtime, 1)))