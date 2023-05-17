# AUTO-GENERATED FILE

from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class InstConfig(SimObject):
    # SimObject type
    type = 'InstConfig'
    # gem5-SALAM attached header
    cxx_header = "hwacc/HWModeling/src/instruction_config.hh"
    ### --- Do Not Modify Below This Line --- ###
    ### Templates
    ## YML Type: instruction
    ## 'llvm_opcodenum' = Param.UInt32(LLVM_OpCodeNum, "LLVM OpCode Enumerated Value")
    ## 'llvm_opcodename' = Param.String(LLVM_OpCodeName, "LLVM OpCode Name")
    ## 'salam_opcode' = Param.String(SALAM_OpCode, "SALAM OpCode Name")
    ## 'runtime_cycles' = Param.UInt32(runtime_cycles, "Instruction Runtime Cycles")
    ## 'stages' = Param.UInt32(stages, "Instruction Functional Unit Stage Count")
    ## '[stage_cycles]' = Param.UInt32([stage_cycles], "Cycles in the Current Stage")
    ## 'functional_unit_limit' = Param.UInt32(functional_unit_limit, "Max Concurrent Functional Unit Scheduling")
    ### -- Code Auto-Generated Below This Line -- ###

class Zext(SimObject):
	# SimObject type
	type = 'Zext'	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/instructions/zext.hh"
	# Instruction params
	functional_unit = Param.UInt32(0, "Default functional unit assignment.")
	functional_unit_limit = Param.UInt32(0, "Default functional unit limit.")
	opcode_num = Param.UInt32(39, "Default instruction llvm enum opcode value.")
	runtime_cycles = Param.UInt32(0, "Default instruction runtime cycles.")

