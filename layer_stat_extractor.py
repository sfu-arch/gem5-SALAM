import sys

BIN_SIZE = sys.argv[1]

layer_count = 0
invert_cycle_count = 0
end_cycle_count = 0
read_count = 0
file_name = "1.txt"
file = open(file_name, 'r', encoding="ISO-8859-1")
for line in file:
    if "Launching push" in line and "count:" in line:
        layer_count += 1
    if "Cycle count end = " in line:
        end_cycle_count = int(line.split('=')[-1])
    if "Cycle count invert = " in line:
        invert_cycle_count = int(line.split('=')[-1])
    if "is_read" in line:
        read_count += 1
print(str(BIN_SIZE) + "," + str(layer_count) + "," + str(end_cycle_count-invert_cycle_count)+','+str(read_count/(end_cycle_count-invert_cycle_count))) 