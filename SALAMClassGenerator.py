


class FunctionalUnitGenerator():
    def __init__(self, bench_directory="", fu_directory=""):
        self.bench_directory = bench_directory
        self.fu_directory = fu_directory
        self.alias = ""
        self.classname = ""
        self.header_name = ""
        self.cxx_header = "src/hwacc/HWModeling/src/functional_units.hh"
        self.cxx_header_py_path = "hwacc/HWModeling/src/functional_units.hh"
        self.fu_base_directory = "src/hwacc/HWModeling/generated/functionalunits/"
        self.fu_base_directory_py_path = "hwacc/HWModeling/generated/functionalunits/"
        self.scons_dir_fu = "src/hwacc/HWModeling/generated/functionalunits/SConscript"
        self.scons_dir_inst = "src/hwacc/HWModeling/generated/instructions/SConscript"

    # Functional unit alias from yml is passed for classname variable
    def set_fu(self, alias=""):
        self.alias = alias
        # Create Python SimObject
        self.classname = ''.join(words.capitalize() for words in self.alias.split('_'))
        # create C++ header
        self.header_name = self.fu_base_directory + self.alias + '.hh'
        self.header_name_py_path = self.fu_base_directory_py_path + self.alias + '.hh'
        self.source_name = self.fu_base_directory + self.alias + '.cc'
        #self.header_name = self.alias + '.hh'
        #self.source_name = self.alias + '.cc'

    def functional_unit_header_generator(self, hwmodel):
        self.hwmodel = hwmodel
        self.new_header = open(self.header_name, 'w')

        # start header gaurd
        self.new_header.write("#ifndef __HWMODEL_" + self.alias.upper() + "_HH__\n")
        self.new_header.write("#define __HWMODEL_" + self.alias.upper() + "_HH__\n\n")

        # warning label
        self.new_header.write("// GENERATED FILE - DO NOT MODIFY\n\n")
        
        # includes
        self.new_header.write("#include \"params/" + self.classname + ".hh\"\n")
        self.new_header.write("#include \"sim/sim_object.hh\"\n")
        self.new_header.write("#include \"base.hh\"\n\n")

        # namespaces
        self.new_header.write("using namespace gem5;\n\n")

        # define class name and inheritance

        self.new_header.write("class " + self.classname + ": public SimObject, public FunctionalUnitBase\n")
        self.new_header.write("{\n")

        # define class body
        # define private members and methods
        self.new_header.write("\tprivate:\n")
        # members

        # methods
        
        # define protected members and methods
        self.new_header.write("\tprotected:\n")
        # members

        # methods

        # define public members and methods
        self.new_header.write("\tpublic:\n")
        # members

        # methods
        # default constructor
        self.new_header.write("\t\t" + self.classname + "();\n")
        # SimObject Constructor
        self.new_header.write("\t\t" + self.classname + "(const " + self.classname + "Params &params);\n")

        # end class body
        self.new_header.write("};\n")
        # end header gaurd
        self.new_header.write("#endif // __HWMODEL_" + self.alias.upper() + "_HH__")

        self.new_header.close()

        # Define source files
        self.new_source = open(self.source_name, 'w')

        # Include header file
        self.new_source.write("#include \"" + self.alias + ".hh\"\n\n")
        
        # Warning
        self.new_source.write("//AUTO-GENERATED FILE\n\n")
        self.new_source.write(self.classname + "::" + self.classname + "(const " + self.classname + "Params &params) :\n")
        self.new_source.write("\tSimObject(params),\n")
        self.new_source.write("\tFunctionalUnitBase( params.alias,\n")
        self.new_source.write("\t\t\t\t\t\tparams.stages,\n")
        self.new_source.write("\t\t\t\t\t\tparams.cycles,\n")
        self.new_source.write("\t\t\t\t\t\tparams.enum_value,\n")
        self.new_source.write("\t\t\t\t\t\tparams.int_size,\n")
        self.new_source.write("\t\t\t\t\t\tparams.int_sign,\n")
        self.new_source.write("\t\t\t\t\t\tparams.int_apmode,\n")
        self.new_source.write("\t\t\t\t\t\tparams.fp_size,\n")
        self.new_source.write("\t\t\t\t\t\tparams.fp_sign,\n")
        self.new_source.write("\t\t\t\t\t\tparams.fp_apmode,\n")
        self.new_source.write("\t\t\t\t\t\tparams.ptr_size,\n")
        self.new_source.write("\t\t\t\t\t\tparams.ptr_sign,\n")
        self.new_source.write("\t\t\t\t\t\tparams.ptr_apmode,\n")
        self.new_source.write("\t\t\t\t\t\tparams.limit,\n")
        self.new_source.write("\t\t\t\t\t\tparams.power_units,\n")
        self.new_source.write("\t\t\t\t\t\tparams.energy_units,\n")
        self.new_source.write("\t\t\t\t\t\tparams.time_units,\n")
        self.new_source.write("\t\t\t\t\t\tparams.area_units,\n")
        self.new_source.write("\t\t\t\t\t\tparams.fu_latency,\n")
        self.new_source.write("\t\t\t\t\t\tparams.internal_power,\n")
        self.new_source.write("\t\t\t\t\t\tparams.switch_power,\n")
        self.new_source.write("\t\t\t\t\t\tparams.dynamic_power,\n")
        self.new_source.write("\t\t\t\t\t\tparams.dynamic_energy,\n")
        self.new_source.write("\t\t\t\t\t\tparams.leakage_power,\n")
        self.new_source.write("\t\t\t\t\t\tparams.area,\n")
        self.new_source.write("\t\t\t\t\t\tparams.path_delay) { }\n")

    def generate_functional_unit_sconscript(self, fu_list = []):
        self.fu_list = fu_list
        with open(self.scons_dir_fu, 'w+') as self.scons_file:
            self.scons_file.write("Import('*')\n")
            self.scons_file.write("if env['TARGET_ISA'] == 'arm':\n\n")
            for unit in self.fu_list:
                self.scons_file.write("    Source('" + unit + ".cc')\n")


    def initalize_fu_list_header(self, fu_list = []):
        self.fu_list = fu_list
        with open(self.cxx_header, 'w+') as self.fu_list_header_file:
            self.fu_list_header_file.write("#ifndef __HWMODEL_FUNCTIONAL_UNITS_HH__\n")
            self.fu_list_header_file.write("#define __HWMODEL_FUNCTIONAL_UNITS_HH__\n\n")
            self.fu_list_header_file.write("#include \"params/FunctionalUnits.hh\"\n")
            self.fu_list_header_file.write("#include \"sim/sim_object.hh\"\n")

            self.fu_list_header_file.write("// GENERATED HEADERS - DO NOT MODIFY\n")
            self.fu_list_header_file.write("#include \"../generated/functionalunits/base.hh\"\n")
            for unit in self.fu_list:
                # Add include headers to files
                self.fu_list_header_file.write("#include \"../generated/functionalunits/" + unit + ".hh\"\n")

            self.fu_list_header_file.write("#include <iostream>\n")
            self.fu_list_header_file.write("#include <cstdlib>\n")
            self.fu_list_header_file.write("#include <vector>\n\n")

            self.fu_list_header_file.write("using namespace gem5;\n\n")

            self.fu_list_header_file.write("class FunctionalUnitBase;\n\n")

            self.fu_list_header_file.write("class FunctionalUnits : public SimObject\n")
            self.fu_list_header_file.write("{\n")
            self.fu_list_header_file.write("\tprivate:\n")
            self.fu_list_header_file.write("\tprotected:\n\n")
            self.fu_list_header_file.write("\tpublic:\n")
            self.fu_list_header_file.write("\t\t// GENERATED CLASS MEMBERS - DO NOT MODIFY\n")
            
            for unit in self.fu_list:
                self.fu_list_header_file.write("\t\t" + ''.join(words.capitalize() for words in unit.split('_')) + "* _" + unit + ";\n")
            #// END OF GENERATED CLASS MEMBERS

            self.fu_list_header_file.write("\t\tFunctionalUnits();\n")
            self.fu_list_header_file.write("\t\t// DEFAULT CONSTRUCTOR - DO NOT MODIFY\n")
            self.fu_list_header_file.write("\t\tFunctionalUnits(const FunctionalUnitsParams &params);\n")
            self.fu_list_header_file.write("\t\t// END DEFAULT CONSTRUCTOR\n")
            self.fu_list_header_file.write("\t\tstd::vector<FunctionalUnitBase*> functional_unit_list;\n")
            self.fu_list_header_file.write("};\n")

            self.fu_list_header_file.write("#endif //__HWMODEL_FUNCTIONAL_UNITS_HH__\n")
            #TODO Write source generator

    def initialize_functional_unit_base_header_file(self):
        self.base_header = self.fu_base_directory + "base.hh"
        with open(self.base_header, 'w+') as self.base_header_file:

            self.base_header_file.write("#ifndef __HWMODEL_FUNCTIONAL_UNIT_BASE_HH__\n")
            self.base_header_file.write("#define __HWMODEL_FUNCTIONAL_UNIT_BASE_HH__\n\n")
            self.base_header_file.write("#include \"../../src/salam_power_model.hh\"\n\n")
            self.base_header_file.write("#include <map>\n")
            self.base_header_file.write("#include <iostream>\n")
            self.base_header_file.write("#include <cstdlib>\n")
            self.base_header_file.write("#include <vector>\n\n")
 
            self.base_header_file.write("class FunctionalUnitBase\n")
            self.base_header_file.write("{\n")

            # define class body
            # define private members and methods
            self.base_header_file.write("\tprivate:\n")
            # members

            # methods
            
            # define protected members and methods
            self.base_header_file.write("\tprotected:\n")
            # members
            self.base_header_file.write("\t\tstd::string _alias;\n")
            self.base_header_file.write("\t\tuint32_t _stages;\n")
            self.base_header_file.write("\t\tuint32_t _cycles;\n")
            self.base_header_file.write("\t\tuint32_t _enum_value;\n")
            self.base_header_file.write("\t\tstd::string _int_size;\n")
            self.base_header_file.write("\t\tstd::string _int_sign;\n")
            self.base_header_file.write("\t\tbool _int_apmode;\n")
            self.base_header_file.write("\t\tstd::string _fp_size;\n")
            self.base_header_file.write("\t\tstd::string _fp_sign;\n")
            self.base_header_file.write("\t\tbool _fp_apmode;\n")
            self.base_header_file.write("\t\tstd::string _ptr_size;\n")
            self.base_header_file.write("\t\tstd::string _ptr_sign;\n")
            self.base_header_file.write("\t\tbool _ptr_apmode;\n")
            self.base_header_file.write("\t\tuint32_t _limit;\n")
            self.base_header_file.write("\t\tstd::string _power_units;\n")
            self.base_header_file.write("\t\tstd::string _energy_units;\n")
            self.base_header_file.write("\t\tstd::string _time_units;\n")
            self.base_header_file.write("\t\tstd::string _area_units;\n")
            self.base_header_file.write("\t\tuint32_t _fu_latency;\n")
            self.base_header_file.write("\t\tdouble _internal_power;\n")
            self.base_header_file.write("\t\tdouble _switch_power;\n")
            self.base_header_file.write("\t\tdouble _dynamic_power;\n")
            self.base_header_file.write("\t\tdouble _dynamic_energy;\n")
            self.base_header_file.write("\t\tdouble _leakage_power;\n")
            self.base_header_file.write("\t\tdouble _area;\n")
            self.base_header_file.write("\t\tdouble _path_delay;\n\n")
            self.base_header_file.write("\t\tuint64_t _available;\n\n")
            self.base_header_file.write("\t\tuint64_t _in_use;\n\n")
            # methods

            # define public members and methods
            self.base_header_file.write("\tpublic:\n")
            # members

            # methods
            # default constructor
            self.base_header_file.write("\t\tFunctionalUnitBase();\n")
            # SimObject Constructor
            self.base_header_file.write("\t\tFunctionalUnitBase( std::string alias,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t stages,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t cycles,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t enum_value,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string int_size,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string int_sign,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tbool int_apmode,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string fp_size,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string fp_sign,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tbool fp_apmode,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string ptr_size,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string ptr_sign,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tbool ptr_apmode,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t limit,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string power_units,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string energy_units,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string time_units,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tstd::string area_units,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t fu_latency,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble internal_power,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble switch_power,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble dynamic_power,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble dynamic_energy,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble leakage_power,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble area,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tdouble path_delay) :\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_alias(alias),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_stages(stages),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_cycles(cycles),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_enum_value(enum_value),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_int_size(int_size),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_int_sign(int_sign),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_int_apmode(int_apmode),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_fp_size(fp_size),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_fp_sign(fp_sign),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_fp_apmode(fp_apmode),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_ptr_size(ptr_size),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_ptr_sign(ptr_sign),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_ptr_apmode(ptr_apmode),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_limit(limit),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_power_units(power_units),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_energy_units(energy_units),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_time_units(time_units),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_area_units(area_units),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_fu_latency(fu_latency),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_internal_power(internal_power),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_switch_power(switch_power),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_dynamic_power(dynamic_power),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_dynamic_energy(dynamic_energy),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_leakage_power(leakage_power),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_area(area),\n")
            self.base_header_file.write("\t\t\t\t\t\t\t_path_delay(path_delay) { }\n")

            # Getters / Setters
            self.base_header_file.write("\t\tstd::string get_alias() { return _alias; }\n")
            self.base_header_file.write("\t\tuint32_t get_stages() { return _stages; }\n")
            self.base_header_file.write("\t\tuint32_t get_cycles() { return _cycles; }\n")
            self.base_header_file.write("\t\tuint32_t get_enum_value() { return _enum_value; }\n")
            self.base_header_file.write("\t\tstd::string get_int_size() { return _int_size; }\n")
            self.base_header_file.write("\t\tstd::string get_int_sign() { return _int_sign; }\n")
            self.base_header_file.write("\t\tbool get_int_apmode() { return _int_apmode; }\n")
            self.base_header_file.write("\t\tstd::string get_fp_size() { return _fp_size; }\n")
            self.base_header_file.write("\t\tstd::string get_fp_sign() { return _fp_sign; }\n")
            self.base_header_file.write("\t\tbool get_fp_apmode() { return _fp_apmode; }\n")
            self.base_header_file.write("\t\tstd::string get_ptr_size() { return _ptr_size; }\n")
            self.base_header_file.write("\t\tstd::string get_ptr_sign() { return _ptr_sign; }\n")
            self.base_header_file.write("\t\tbool get_ptr_apmode() { return _ptr_apmode; }\n")
            self.base_header_file.write("\t\tuint32_t get_limit() { return _limit; }\n")
            self.base_header_file.write("\t\tstd::string get_power_units() { return _power_units; }\n")
            self.base_header_file.write("\t\tstd::string get_energy_units() { return _energy_units; }\n")
            self.base_header_file.write("\t\tstd::string get_time_units() { return _time_units; }\n")
            self.base_header_file.write("\t\tstd::string get_area_units() { return _area_units; }\n")
            self.base_header_file.write("\t\tuint32_t get_fu_latency() { return _fu_latency; }\n")
            self.base_header_file.write("\t\tdouble get_internal_power() { return _internal_power; }\n")
            self.base_header_file.write("\t\tdouble get_switch_power() { return _switch_power; }\n")
            self.base_header_file.write("\t\tdouble get_dynamic_power() { return _dynamic_power; }\n")
            self.base_header_file.write("\t\tdouble get_dynamic_energy() { return _dynamic_energy; }\n")
            self.base_header_file.write("\t\tdouble get_leakage_power() { return _leakage_power; }\n")
            self.base_header_file.write("\t\tdouble get_area() { return _area; }\n")
            self.base_header_file.write("\t\tdouble get_path_delay() { return _path_delay; }\n")
            self.base_header_file.write("\t\tbool is_available() { return (_in_use >= _available); }\n")
            self.base_header_file.write("\t\tvoid use_functional_unit() { _in_use++; }\n")
            self.base_header_file.write("\t\tvoid clear_functional_unit() { _in_use--; }\n")
            self.base_header_file.write("\t\tvoid set_functional_unit_limit(uint64_t available) { _available = available; }\n")
            self.base_header_file.write("\t\tvoid inc_functional_unit_limit() { _available++; }\n")
            self.base_header_file.write("\t\tuint64_t get_functional_unit_limit() { return _available; }\n\n")


            # end class body
            self.base_header_file.write("};\n")
            # end header gaurd
            self.base_header_file.write("#endif // __HWMODEL_FUNCTIONAL_UNIT_BASE_HH__")

            self.base_header_file.close()

    def initialize_simobject_file(self, fu_list = []):
        self.fu_list = fu_list
        with open(self.fu_directory, 'w+') as self.simobject_file:
            # Warning 
            self.simobject_file.write("# AUTO-GENERATED FILE\n\n")
            # imports
            self.simobject_file.write("from m5.params import *\n")
            self.simobject_file.write("from m5.proxy import *\n")
            self.simobject_file.write("from m5.SimObject import SimObject\n\n")
            # define the class
            self.simobject_file.write("class FunctionalUnits(SimObject):\n")
            self.simobject_file.write("\t# SimObject type\n")
            self.simobject_file.write("\ttype = 'FunctionalUnits'\n")
            self.simobject_file.write("\t# gem5-SALAM attached header\n")
            self.simobject_file.write("\tcxx_header = \"" + self.cxx_header_py_path + "\"\n\n")

            for unit in self.fu_list:
                self.simobject_file.write("\t" + unit + " = Param." + ''.join(words.capitalize() for words in unit.split('_')))
                self.simobject_file.write("(Parent.any, \"" + unit + " functional unit SimObject.\")\n")

            self.simobject_file.write("\n#AUTO-GENERATED CLASSES\n")

    def simobject_generator(self, hwmodel):
        self.hwmodel = hwmodel
        # create Python SimObject
        #self.simobject_file = self.directory + '/' + self.alias + '.py'
        #self.simobject_file = self.classname + '.py'
        with open(self.fu_directory, 'a+') as self.simobject_file:
            self.simobject_file.write("class " + self.classname + "(SimObject):\n")
            self.simobject_file.write("\t# SimObject type\n")
            self.simobject_file.write("\ttype = '" + self.classname + "'\n")
            self.simobject_file.write("\t# gem5-SALAM attached header\n")
            self.simobject_file.write("\tcxx_header = \"" + self.header_name_py_path + "\"\n")
            self.simobject_file.write("\t#HW Params\n")
            self.simobject_file.write("\talias = Param.String(\"" + self.alias + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tstages = Param.UInt32(" + str(self.hwmodel.stages) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tcycles = Param.UInt32(" + str(self.hwmodel.cycles) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tenum_value = Param.UInt32(" + str(self.hwmodel.enum_value) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tint_size = Param.String(\"" + str(self.hwmodel.int_size) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tint_sign = Param.String(\"" + str(self.hwmodel.int_sign) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tint_apmode = Param.Bool(" + str(self.hwmodel.int_apmode) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tfp_size = Param.String(\"" + str(self.hwmodel.fp_size) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tfp_sign = Param.String(\"" + str(self.hwmodel.fp_sign) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tfp_apmode = Param.Bool(" + str(self.hwmodel.fp_apmode) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tptr_size = Param.String(\"" + str(self.hwmodel.ptr_size) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tptr_sign = Param.String(\"" + str(self.hwmodel.ptr_sign) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tptr_apmode = Param.Bool(" + str(self.hwmodel.ptr_apmode) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tlimit = Param.UInt32(" + str(self.hwmodel.limit) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\t#Power Params\n")
            self.simobject_file.write("\tpower_units = Param.String(\"" + str(self.hwmodel.power_units) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tenergy_units = Param.String(\"" + str(self.hwmodel.energy_units) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\ttime_units = Param.String(\"" + str(self.hwmodel.time_units) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tarea_units = Param.String(\"" + str(self.hwmodel.area_units) + "\", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tfu_latency = Param.UInt32(" + str(self.hwmodel.fu_latency) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tinternal_power = Param.UInt32(" + str(self.hwmodel.internal_power) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tswitch_power = Param.UInt32(" + str(self.hwmodel.switch_power) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tdynamic_power = Param.UInt32(" + str(self.hwmodel.dynamic_power) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tdynamic_energy = Param.UInt32(" + str(self.hwmodel.dynamic_energy) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tleakage_power = Param.UInt32(" + str(self.hwmodel.leakage_power) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tarea = Param.UInt32(" + str(self.hwmodel.area) + ", \"Default values set from " + self.alias + ".yml\")\n")
            self.simobject_file.write("\tpath_delay = Param.UInt32(" + str(self.hwmodel.path_delay) + ", \"Default values set from " + self.alias + ".yml\")\n\n")

    def instruction_simobject(self, instruction):
        self.functional_unit = instruction['functional_unit']
        self.functional_unit_limit = instruction['functional_unit_limit']
        self.opcode_num = instruction['opcode_num']
        self.runtime_cycles = instruction['runtime_cycles']


    def instruction_simobject_generator(self, hwmodel):
        self.hwmodel = hwmodel
        self.inst_dict = hwmodel.get_instruction_list()
        with open("src/hwacc/InstConfig.py", 'w+') as self.simobject_file:
            # Warning 
            self.simobject_file.write("# AUTO-GENERATED FILE\n\n")
            # imports
            self.simobject_file.write("from m5.params import *\n")
            self.simobject_file.write("from m5.proxy import *\n")
            self.simobject_file.write("from m5.SimObject import SimObject\n\n")
            # define the class
            self.simobject_file.write("class InstConfig(SimObject):\n")
            self.simobject_file.write("\t# SimObject type\n")
            self.simobject_file.write("\ttype = 'InstConfig'\n")
            self.simobject_file.write("\t# gem5-SALAM attached header\n")
            self.simobject_file.write("\tcxx_header = \"hwacc/HWModeling/src/instruction_config.hh\"\n\n")
            for inst_name in self.inst_dict['instructions'].keys():
                self.simobject_file.write("\t" + str(inst_name) + " = Param." + ''.join(words.capitalize() for words in inst_name.split('_')) + "(Parent.any, \"" + str(inst_name) + " instruction SimObject\")\n")
            self.simobject_file.write("\n#AUTO-GENERATED CLASSES\n")
            for inst_name in self.inst_dict['instructions'].keys():
                self.simobject_file.write("class " + ''.join(words.capitalize() for words in inst_name.split('_')) + "(SimObject):\n")
                self.simobject_file.write("\t# SimObject type\n")
                self.simobject_file.write("\ttype = \'" + ''.join(words.capitalize() for words in inst_name.split('_')) + "\'")
                self.simobject_file.write("\t# gem5-SALAM attached header\n")
                self.simobject_file.write("\tcxx_header = \"hwacc/HWModeling/generated/instructions/" + str(inst_name) + ".hh\"\n")
                self.simobject_file.write("\t# Instruction params\n")
                self.simobject_file.write("\tfunctional_unit = Param.UInt32(" + str(self.inst_dict['instructions'][inst_name]['functional_unit']) + ", \"Default functional unit assignment.\")\n")
                self.simobject_file.write("\tfunctional_unit_limit = Param.UInt32(" + str(self.inst_dict['instructions'][inst_name]['functional_unit_limit']) + ", \"Default functional unit limit.\")\n")
                self.simobject_file.write("\topcode_num = Param.UInt32(" + str(self.inst_dict['instructions'][inst_name]['opcode_num']) + ", \"Default instruction llvm enum opcode value.\")\n")
                self.simobject_file.write("\truntime_cycles = Param.UInt32(" + str(self.inst_dict['instructions'][inst_name]['runtime_cycles']) + ", \"Default instruction runtime cycles.\")\n\n")

    def initalize_inst_config_header(self, inst_list = []):
        self.inst_list = inst_list['instructions']
        with open("src/hwacc/HWModeling/src/instruction_config.hh", 'w+') as self.inst_config_header_file:
            self.inst_config_header_file.write("#ifndef __HWMODEL_INSTRUCTION_CONFIG_HH__\n")
            self.inst_config_header_file.write("#define __HWMODEL_INSTRUCTION_CONFIG_HH__\n\n")
            self.inst_config_header_file.write("#include \"params/InstConfig.hh\"\n")
            self.inst_config_header_file.write("#include \"sim/sim_object.hh\"\n")

            self.inst_config_header_file.write("// GENERATED HEADERS - DO NOT MODIFY\n")
            self.inst_config_header_file.write("#include \"../generated/instructions/base.hh\"\n")
            for inst in self.inst_list:
                # Add include headers to files
                self.inst_config_header_file.write("#include \"../generated/instructions/" + inst + ".hh\"\n")

            self.inst_config_header_file.write("#include <iostream>\n")
            self.inst_config_header_file.write("#include <cstdlib>\n")
            self.inst_config_header_file.write("#include <vector>\n\n")

            self.inst_config_header_file.write("using namespace gem5;\n\n")

            self.inst_config_header_file.write("class InstConfigBase;\n\n")

            self.inst_config_header_file.write("class InstConfig : public SimObject\n")
            self.inst_config_header_file.write("{\n")
            self.inst_config_header_file.write("\tprivate:\n")
            self.inst_config_header_file.write("\tprotected:\n\n")
            self.inst_config_header_file.write("\tpublic:\n")
            self.inst_config_header_file.write("\t\t// GENERATED CLASS MEMBERS - DO NOT MODIFY\n")
            
            for inst in self.inst_list:
                self.inst_config_header_file.write("\t\t" + ''.join(words.capitalize() for words in inst.split('_')) + "* _" + inst + ";\n")
            #// END OF GENERATED CLASS MEMBERS

            self.inst_config_header_file.write("\t\tInstConfig();\n")
            self.inst_config_header_file.write("\t\t// DEFAULT CONSTRUCTOR - DO NOT MODIFY\n")
            self.inst_config_header_file.write("\t\tInstConfig(const InstConfigParams &params);\n")
            self.inst_config_header_file.write("\t\t// END DEFAULT CONSTRUCTOR\n")
            self.inst_config_header_file.write("\t\tstd::vector<InstConfigBase*> inst_list;")
            self.inst_config_header_file.write("};\n")

            self.inst_config_header_file.write("#endif //__INSTRUCTION_CONFIG_HH__\n")


    def inst_config_header_generator(self, inst_params = { }):
        self.inst_params = inst_params
        self.header_name = "src/hwacc/HWModeling/generated/instructions/" + str(inst_params) + ".hh"
        self.source_name = "src/hwacc/HWModeling/generated/instructions/" + str(inst_params) + ".cc"

        self.new_header = open(self.header_name, 'w')
        self.classname = ''.join(words.capitalize() for words in str(inst_params).split('_'))

        # start header gaurd
        self.new_header.write("#ifndef __HWMODEL_" + str(inst_params).upper() + "_HH__\n")
        self.new_header.write("#define __HWMODEL_" + str(inst_params).upper() + "_HH__\n\n")

        # warning label
        self.new_header.write("// GENERATED FILE - DO NOT MODIFY\n\n")
        
        # includes
        self.new_header.write("#include \"params/" + self.classname + ".hh\"\n")
        self.new_header.write("#include \"sim/sim_object.hh\"\n")
        self.new_header.write("#include \"base.hh\"\n\n")

        # namespaces
        self.new_header.write("using namespace gem5;\n\n")

        # define class name and inheritance

        self.new_header.write("class " + self.classname + ": public SimObject, public InstConfigBase\n")
        self.new_header.write("{\n")

        # define class body
        # define private members and methods
        self.new_header.write("\tprivate:\n")
        # members

        # methods
        
        # define protected members and methods
        self.new_header.write("\tprotected:\n")
        # members

        # methods

        # define public members and methods
        self.new_header.write("\tpublic:\n")
        # members

        # methods
        # default constructor
        self.new_header.write("\t\t" + self.classname + "();\n")
        # SimObject Constructor
        self.new_header.write("\t\t" + self.classname + "(const " + self.classname + "Params &params);\n")

        # end class body
        self.new_header.write("};\n")
        # end header gaurd
        self.new_header.write("#endif // __HWMODEL_" + str(inst_params).upper() + "_HH__")

        self.new_header.close()

        # Define source files
        self.new_source = open(self.source_name, 'w')

        # Include header file
        self.new_source.write("#include \"" + str(inst_params) + ".hh\"\n\n")
        
        # Warning
        self.new_source.write("//AUTO-GENERATED FILE\n\n")
        self.new_source.write(self.classname + "::" + self.classname + "(const " + self.classname + "Params &params) :\n")
        self.new_source.write("\tSimObject(params),\n")
        self.new_source.write("\tInstConfigBase( params.functional_unit,\n")
        self.new_source.write("\t\t\t\t\t\tparams.functional_unit_limit,\n")
        self.new_source.write("\t\t\t\t\t\tparams.opcode_num,\n")
        self.new_source.write("\t\t\t\t\t\tparams.runtime_cycles) { }\n")

    def initialize_inst_config_base_header_file(self):
        self.base_header = "src/hwacc/HWModeling/generated/instructions/base.hh"
        with open(self.base_header, 'w+') as self.base_header_file:

            self.base_header_file.write("#ifndef __HWMODEL_INST_CONFIG_BASE_HH__\n")
            self.base_header_file.write("#define __HWMODEL_INST_CONFIG_BASE_HH__\n\n")
            self.base_header_file.write("#include <map>\n")
            self.base_header_file.write("#include <iostream>\n")
            self.base_header_file.write("#include <cstdlib>\n")
            self.base_header_file.write("#include <vector>\n\n")
 
            self.base_header_file.write("class InstConfigBase\n")
            self.base_header_file.write("{\n")

            # define class body
            # define private members and methods
            self.base_header_file.write("\tprivate:\n")
            # members

            # methods
            
            # define protected members and methods
            self.base_header_file.write("\tprotected:\n")
            # members
            self.base_header_file.write("\t\tuint32_t _functional_unit;\n")
            self.base_header_file.write("\t\tuint32_t _functional_unit_limit;\n")
            self.base_header_file.write("\t\tuint32_t _opcode_num;\n")
            self.base_header_file.write("\t\tuint32_t _runtime_cycles;\n")

            # methods

            # define public members and methods
            self.base_header_file.write("\tpublic:\n")
            # members

            # methods
            # default constructor
            self.base_header_file.write("\t\tInstConfigBase();\n")
            # SimObject Constructor
            self.base_header_file.write("\t\tInstConfigBase( uint32_t functional_unit,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t functional_unit_limit,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t opcode_num,\n")
            self.base_header_file.write("\t\t\t\t\t\t\tuint32_t runtime_cycles) { }\n")

            # Getters / Setters
            self.base_header_file.write("\t\tuint32_t get_functional_unit() { return _functional_unit; }\n")
            self.base_header_file.write("\t\tuint32_t get_functional_unit_limit() { return _functional_unit_limit; }\n")
            self.base_header_file.write("\t\tuint32_t get_opcode_num() { return _opcode_num; }\n")
            self.base_header_file.write("\t\tuint32_t get_runtime_cycles() { return _runtime_cycles; }\n")
            
            # end class body
            self.base_header_file.write("};\n")
            # end header gaurd
            self.base_header_file.write("#endif // __HWMODEL_INST_CONFIG_BASE_HH__")

            self.base_header_file.close()

    def generate_inst_config_sconscript(self, inst_list = []):
        self.inst_list = inst_list
        with open(self.scons_dir_inst, 'w+') as self.scons_file:
            self.scons_file.write("Import('*')\n")
            self.scons_file.write("if env['TARGET_ISA'] == 'arm':\n\n")
            for inst in self.inst_list:
                self.scons_file.write("    Source('" + inst + ".cc')\n")