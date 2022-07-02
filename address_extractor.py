import sys

dir = sys.argv[1]
filename = 'log_ad_512'
file_loc = dir + filename + '.txt'
mode = 'F'
addresses = {'F': [], 'R': []}

file = open(file_loc, 'r', encoding="ISO-8859-1")
for line in file:
    if 'Found invert' in line:
        mode = 'R'
    if 'cluster_cache' in line: 
        addr = ''
        if 'MISS' in line:
            addr = line.split('MISS')[-1].strip()
        if 'HIT' in line:
            addr = line.split('HIT')[-1].strip()
        addresses[mode].append(addr)
outfile = open(dir + 'addresses.txt', 'w')
for mode in addresses:
    for addr in addresses[mode]:
        outfile.write(mode + '_' + addr + '\n')
outfile.close()