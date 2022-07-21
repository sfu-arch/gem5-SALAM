from distutils.command.config import config
import m5
from m5.objects import *
from m5.util import *
from configparser import ConfigParser

def AccConfig(acc, config_file, bench_file):
    # Setup config file parser
    Config = ConfigParser()
    Config.read((config_file))
    Config.sections()
    def ConfigSectionMap(section):
        dict1 = {}
        options = Config.options(section)
        for option in options:
            try:
                dict1[option] = Config.get(section, option)
                if dict1[option] == -1:
                    DebugPrint("skip: %s" % option)
            except:
                print("exception on %s!" % option)
                dict1[option] = None
        return dict1
    # Setup comm interface
    acc.clock_period = ConfigSectionMap("AccConfig")['clock_period']
    predef = ConfigSectionMap("AccConfig")['premap_data']

    if (predef == "1" or predef == "True"):
        acc.premap_data = predef
        acc.data_bases = ConfigSectionMap("AccConfig")['data_bases']


    # Initialize LLVMInterface Objects
    acc.llvm_interface = LLVMInterface()

    # Benchmark path
    acc.llvm_interface.in_file = bench_file

    # Set scheduling constraints
    acc.llvm_interface.sched_threshold = ConfigSectionMap("Scheduler")['sched_threshold']
    acc.llvm_interface.clock_period = ConfigSectionMap("AccConfig")['clock_period']
    acc.llvm_interface.lockstep_mode = Config.getboolean("Scheduler", 'lockstep_mode')

	# Initialize HWInterface Objects
    acc.hw_interface = HWInterface()
    acc.hw_interface.cycle_counts = CycleCounts()
    acc.hw_interface.functional_units = FunctionalUnits()

    #acc.hw_interface.functional_units.int_adder = Adder()
    acc.hw_interface.inst_config = InstConfig()
    #acc.hw_interface.inst_config.add_config = Add()
    acc.hw_interface.salam_power_model = SALAMPowerModel()
    acc.hw_interface.hw_statistics = HWStatistics()
    acc.hw_interface.simulator_config = SimulatorConfig()
    acc.hw_interface.opcodes = InstOpCodes()

def AccSPMConfig(acc, spm, config_file):
    # Setup config file parser
    Config = ConfigParser.ConfigParser()
    Config.read((config_file))
    Config.sections()
    def ConfigSectionMap(section):
        dict1 = {}
        options = Config.options(section)
        for option in options:
            try:
                dict1[option] = Config.get(section, option)
                if dict1[option] == -1:
                    DebugPrint("skip: %s" % option)
            except:
                print("exception on %s!" % option)
                dict1[option] = None
        return dict1

    spm.range = AddrRange(ConfigSectionMap("Memory")['addr_range'], \
                          size=ConfigSectionMap("Memory")['size'])
    spm.latency = ConfigSectionMap("Memory")['latency']
    spm.conf_table_reported = False
    spm.ready_mode = Config.getboolean("Memory", 'ready_mode')
    spm.reset_on_scratchpad_read = Config.getboolean("Memory", 'reset_on_private_read')
    num_ports = ConfigSectionMap("Memory")['ports']
    for i in range(int(num_ports)):
        acc.spm[i] = spm.spm_ports[i]
