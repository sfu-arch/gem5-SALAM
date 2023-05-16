import sys

from yaml import load, Loader
benchmark_dir = sys.argv[1]

with open(benchmark_dir+"/config.yml", "r") as stream: 
    data = load(stream, Loader)
acc_cluster = data['acc_cluster']

for entry in acc_cluster:
    if 'Accelerator' in entry:
        name = entry['Accelerator'][0]['Name']
        if name.lower() != "top":
            if 'LocalSlaves' not in entry['Accelerator'][0]:
                raise Exception("LocalSlaves should not be specified for accelerator %s" % name)
            if len(entry['Accelerator']) < 2 or "Var" not in entry['Accelerator'][1] or entry['Accelerator'][1]['Var'][0]['Type'].lower() != "spm":
                raise Exception("Scratchpad should be specified for accelerator %s" % name)