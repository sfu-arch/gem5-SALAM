# AUTO-GENERATED FILE

from m5.params import *
from m5.proxy import *
from m5.SimObject import SimObject

class FunctionalUnits(SimObject):
	# SimObject type
	type = 'FunctionalUnits'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/src/functional_units.hh"

	double_multiplier = Param.DoubleMultiplier(Parent.any, "double_multiplier functional unit SimObject.")
	bit_register = Param.BitRegister(Parent.any, "bit_register functional unit SimObject.")
	bitwise_operations = Param.BitwiseOperations(Parent.any, "bitwise_operations functional unit SimObject.")
	double_adder = Param.DoubleAdder(Parent.any, "double_adder functional unit SimObject.")
	float_divider = Param.FloatDivider(Parent.any, "float_divider functional unit SimObject.")
	bit_shifter = Param.BitShifter(Parent.any, "bit_shifter functional unit SimObject.")
	integer_multiplier = Param.IntegerMultiplier(Parent.any, "integer_multiplier functional unit SimObject.")
	integer_adder = Param.IntegerAdder(Parent.any, "integer_adder functional unit SimObject.")
	double_divider = Param.DoubleDivider(Parent.any, "double_divider functional unit SimObject.")
	float_adder = Param.FloatAdder(Parent.any, "float_adder functional unit SimObject.")
	float_multiplier = Param.FloatMultiplier(Parent.any, "float_multiplier functional unit SimObject.")

#AUTO-GENERATED CLASSES
class DoubleMultiplier(SimObject):
	# SimObject type
	type = 'DoubleMultiplier'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/double_multiplier.hh"
	#HW Params
	alias = Param.String("double_multiplier", "Default values set from double_multiplier.yml")
	stages = Param.UInt32(3, "Default values set from double_multiplier.yml")
	cycles = Param.UInt32(5, "Default values set from double_multiplier.yml")
	enum_value = Param.UInt32(9, "Default values set from double_multiplier.yml")
	int_size = Param.String("none", "Default values set from double_multiplier.yml")
	int_sign = Param.String("none", "Default values set from double_multiplier.yml")
	int_apmode = Param.Bool(False, "Default values set from double_multiplier.yml")
	fp_size = Param.String("double", "Default values set from double_multiplier.yml")
	fp_sign = Param.String("any", "Default values set from double_multiplier.yml")
	fp_apmode = Param.Bool(True, "Default values set from double_multiplier.yml")
	ptr_size = Param.String("none", "Default values set from double_multiplier.yml")
	ptr_sign = Param.String("none", "Default values set from double_multiplier.yml")
	ptr_apmode = Param.Bool(False, "Default values set from double_multiplier.yml")
	limit = Param.UInt32(0, "Default values set from double_multiplier.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from double_multiplier.yml")
	energy_units = Param.String("pJ", "Default values set from double_multiplier.yml")
	time_units = Param.String("ns", "Default values set from double_multiplier.yml")
	area_units = Param.String("um^2", "Default values set from double_multiplier.yml")
	fu_latency = Param.UInt32(5, "Default values set from double_multiplier.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from double_multiplier.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from double_multiplier.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from double_multiplier.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from double_multiplier.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from double_multiplier.yml")
	area = Param.UInt32(5.981433, "Default values set from double_multiplier.yml")
	path_delay = Param.UInt32(1.75, "Default values set from double_multiplier.yml")

class BitRegister(SimObject):
	# SimObject type
	type = 'BitRegister'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/bit_register.hh"
	#HW Params
	alias = Param.String("bit_register", "Default values set from bit_register.yml")
	stages = Param.UInt32(0, "Default values set from bit_register.yml")
	cycles = Param.UInt32(0, "Default values set from bit_register.yml")
	enum_value = Param.UInt32(15, "Default values set from bit_register.yml")
	int_size = Param.String("any", "Default values set from bit_register.yml")
	int_sign = Param.String("any", "Default values set from bit_register.yml")
	int_apmode = Param.Bool(True, "Default values set from bit_register.yml")
	fp_size = Param.String("any", "Default values set from bit_register.yml")
	fp_sign = Param.String("any", "Default values set from bit_register.yml")
	fp_apmode = Param.Bool(True, "Default values set from bit_register.yml")
	ptr_size = Param.String("any", "Default values set from bit_register.yml")
	ptr_sign = Param.String("any", "Default values set from bit_register.yml")
	ptr_apmode = Param.Bool(True, "Default values set from bit_register.yml")
	limit = Param.UInt32(0, "Default values set from bit_register.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from bit_register.yml")
	energy_units = Param.String("pJ", "Default values set from bit_register.yml")
	time_units = Param.String("ns", "Default values set from bit_register.yml")
	area_units = Param.String("um^2", "Default values set from bit_register.yml")
	fu_latency = Param.UInt32(5, "Default values set from bit_register.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from bit_register.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from bit_register.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from bit_register.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from bit_register.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from bit_register.yml")
	area = Param.UInt32(5.981433, "Default values set from bit_register.yml")
	path_delay = Param.UInt32(1.75, "Default values set from bit_register.yml")

class BitwiseOperations(SimObject):
	# SimObject type
	type = 'BitwiseOperations'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/bitwise_operations.hh"
	#HW Params
	alias = Param.String("bitwise_operations", "Default values set from bitwise_operations.yml")
	stages = Param.UInt32(1, "Default values set from bitwise_operations.yml")
	cycles = Param.UInt32(1, "Default values set from bitwise_operations.yml")
	enum_value = Param.UInt32(4, "Default values set from bitwise_operations.yml")
	int_size = Param.String("any", "Default values set from bitwise_operations.yml")
	int_sign = Param.String("any", "Default values set from bitwise_operations.yml")
	int_apmode = Param.Bool(True, "Default values set from bitwise_operations.yml")
	fp_size = Param.String("none", "Default values set from bitwise_operations.yml")
	fp_sign = Param.String("none", "Default values set from bitwise_operations.yml")
	fp_apmode = Param.Bool(False, "Default values set from bitwise_operations.yml")
	ptr_size = Param.String("none", "Default values set from bitwise_operations.yml")
	ptr_sign = Param.String("none", "Default values set from bitwise_operations.yml")
	ptr_apmode = Param.Bool(False, "Default values set from bitwise_operations.yml")
	limit = Param.UInt32(0, "Default values set from bitwise_operations.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from bitwise_operations.yml")
	energy_units = Param.String("pJ", "Default values set from bitwise_operations.yml")
	time_units = Param.String("ns", "Default values set from bitwise_operations.yml")
	area_units = Param.String("um^2", "Default values set from bitwise_operations.yml")
	fu_latency = Param.UInt32(5, "Default values set from bitwise_operations.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from bitwise_operations.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from bitwise_operations.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from bitwise_operations.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from bitwise_operations.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from bitwise_operations.yml")
	area = Param.UInt32(5.981433, "Default values set from bitwise_operations.yml")
	path_delay = Param.UInt32(1.75, "Default values set from bitwise_operations.yml")

class DoubleAdder(SimObject):
	# SimObject type
	type = 'DoubleAdder'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/double_adder.hh"
	#HW Params
	alias = Param.String("double_adder", "Default values set from double_adder.yml")
	stages = Param.UInt32(3, "Default values set from double_adder.yml")
	cycles = Param.UInt32(5, "Default values set from double_adder.yml")
	enum_value = Param.UInt32(6, "Default values set from double_adder.yml")
	int_size = Param.String("none", "Default values set from double_adder.yml")
	int_sign = Param.String("none", "Default values set from double_adder.yml")
	int_apmode = Param.Bool(False, "Default values set from double_adder.yml")
	fp_size = Param.String("double", "Default values set from double_adder.yml")
	fp_sign = Param.String("any", "Default values set from double_adder.yml")
	fp_apmode = Param.Bool(True, "Default values set from double_adder.yml")
	ptr_size = Param.String("none", "Default values set from double_adder.yml")
	ptr_sign = Param.String("none", "Default values set from double_adder.yml")
	ptr_apmode = Param.Bool(False, "Default values set from double_adder.yml")
	limit = Param.UInt32(0, "Default values set from double_adder.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from double_adder.yml")
	energy_units = Param.String("pJ", "Default values set from double_adder.yml")
	time_units = Param.String("ns", "Default values set from double_adder.yml")
	area_units = Param.String("um^2", "Default values set from double_adder.yml")
	fu_latency = Param.UInt32(5, "Default values set from double_adder.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from double_adder.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from double_adder.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from double_adder.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from double_adder.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from double_adder.yml")
	area = Param.UInt32(5.981433, "Default values set from double_adder.yml")
	path_delay = Param.UInt32(1.75, "Default values set from double_adder.yml")

class FloatDivider(SimObject):
	# SimObject type
	type = 'FloatDivider'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/float_divider.hh"
	#HW Params
	alias = Param.String("float_divider", "Default values set from float_divider.yml")
	stages = Param.UInt32(3, "Default values set from float_divider.yml")
	cycles = Param.UInt32(16, "Default values set from float_divider.yml")
	enum_value = Param.UInt32(8, "Default values set from float_divider.yml")
	int_size = Param.String("none", "Default values set from float_divider.yml")
	int_sign = Param.String("none", "Default values set from float_divider.yml")
	int_apmode = Param.Bool(False, "Default values set from float_divider.yml")
	fp_size = Param.String("single", "Default values set from float_divider.yml")
	fp_sign = Param.String("any", "Default values set from float_divider.yml")
	fp_apmode = Param.Bool(True, "Default values set from float_divider.yml")
	ptr_size = Param.String("none", "Default values set from float_divider.yml")
	ptr_sign = Param.String("none", "Default values set from float_divider.yml")
	ptr_apmode = Param.Bool(False, "Default values set from float_divider.yml")
	limit = Param.UInt32(0, "Default values set from float_divider.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from float_divider.yml")
	energy_units = Param.String("pJ", "Default values set from float_divider.yml")
	time_units = Param.String("ns", "Default values set from float_divider.yml")
	area_units = Param.String("um^2", "Default values set from float_divider.yml")
	fu_latency = Param.UInt32(5, "Default values set from float_divider.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from float_divider.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from float_divider.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from float_divider.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from float_divider.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from float_divider.yml")
	area = Param.UInt32(5.981433, "Default values set from float_divider.yml")
	path_delay = Param.UInt32(1.75, "Default values set from float_divider.yml")

class BitShifter(SimObject):
	# SimObject type
	type = 'BitShifter'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/bit_shifter.hh"
	#HW Params
	alias = Param.String("bit_shifter", "Default values set from bit_shifter.yml")
	stages = Param.UInt32(1, "Default values set from bit_shifter.yml")
	cycles = Param.UInt32(1, "Default values set from bit_shifter.yml")
	enum_value = Param.UInt32(3, "Default values set from bit_shifter.yml")
	int_size = Param.String("any", "Default values set from bit_shifter.yml")
	int_sign = Param.String("any", "Default values set from bit_shifter.yml")
	int_apmode = Param.Bool(True, "Default values set from bit_shifter.yml")
	fp_size = Param.String("none", "Default values set from bit_shifter.yml")
	fp_sign = Param.String("none", "Default values set from bit_shifter.yml")
	fp_apmode = Param.Bool(False, "Default values set from bit_shifter.yml")
	ptr_size = Param.String("none", "Default values set from bit_shifter.yml")
	ptr_sign = Param.String("none", "Default values set from bit_shifter.yml")
	ptr_apmode = Param.Bool(False, "Default values set from bit_shifter.yml")
	limit = Param.UInt32(0, "Default values set from bit_shifter.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from bit_shifter.yml")
	energy_units = Param.String("pJ", "Default values set from bit_shifter.yml")
	time_units = Param.String("ns", "Default values set from bit_shifter.yml")
	area_units = Param.String("um^2", "Default values set from bit_shifter.yml")
	fu_latency = Param.UInt32(5, "Default values set from bit_shifter.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from bit_shifter.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from bit_shifter.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from bit_shifter.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from bit_shifter.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from bit_shifter.yml")
	area = Param.UInt32(5.981433, "Default values set from bit_shifter.yml")
	path_delay = Param.UInt32(1.75, "Default values set from bit_shifter.yml")

class IntegerMultiplier(SimObject):
	# SimObject type
	type = 'IntegerMultiplier'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/integer_multiplier.hh"
	#HW Params
	alias = Param.String("integer_multiplier", "Default values set from integer_multiplier.yml")
	stages = Param.UInt32(1, "Default values set from integer_multiplier.yml")
	cycles = Param.UInt32(1, "Default values set from integer_multiplier.yml")
	enum_value = Param.UInt32(2, "Default values set from integer_multiplier.yml")
	int_size = Param.String("any", "Default values set from integer_multiplier.yml")
	int_sign = Param.String("any", "Default values set from integer_multiplier.yml")
	int_apmode = Param.Bool(True, "Default values set from integer_multiplier.yml")
	fp_size = Param.String("none", "Default values set from integer_multiplier.yml")
	fp_sign = Param.String("none", "Default values set from integer_multiplier.yml")
	fp_apmode = Param.Bool(False, "Default values set from integer_multiplier.yml")
	ptr_size = Param.String("none", "Default values set from integer_multiplier.yml")
	ptr_sign = Param.String("none", "Default values set from integer_multiplier.yml")
	ptr_apmode = Param.Bool(False, "Default values set from integer_multiplier.yml")
	limit = Param.UInt32(0, "Default values set from integer_multiplier.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from integer_multiplier.yml")
	energy_units = Param.String("pJ", "Default values set from integer_multiplier.yml")
	time_units = Param.String("ns", "Default values set from integer_multiplier.yml")
	area_units = Param.String("um^2", "Default values set from integer_multiplier.yml")
	fu_latency = Param.UInt32(5, "Default values set from integer_multiplier.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from integer_multiplier.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from integer_multiplier.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from integer_multiplier.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from integer_multiplier.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from integer_multiplier.yml")
	area = Param.UInt32(5.981433, "Default values set from integer_multiplier.yml")
	path_delay = Param.UInt32(1.75, "Default values set from integer_multiplier.yml")

class IntegerAdder(SimObject):
	# SimObject type
	type = 'IntegerAdder'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/integer_adder.hh"
	#HW Params
	alias = Param.String("integer_adder", "Default values set from integer_adder.yml")
	stages = Param.UInt32(1, "Default values set from integer_adder.yml")
	cycles = Param.UInt32(1, "Default values set from integer_adder.yml")
	enum_value = Param.UInt32(1, "Default values set from integer_adder.yml")
	int_size = Param.String("any", "Default values set from integer_adder.yml")
	int_sign = Param.String("any", "Default values set from integer_adder.yml")
	int_apmode = Param.Bool(True, "Default values set from integer_adder.yml")
	fp_size = Param.String("none", "Default values set from integer_adder.yml")
	fp_sign = Param.String("none", "Default values set from integer_adder.yml")
	fp_apmode = Param.Bool(False, "Default values set from integer_adder.yml")
	ptr_size = Param.String("none", "Default values set from integer_adder.yml")
	ptr_sign = Param.String("none", "Default values set from integer_adder.yml")
	ptr_apmode = Param.Bool(False, "Default values set from integer_adder.yml")
	limit = Param.UInt32(0, "Default values set from integer_adder.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from integer_adder.yml")
	energy_units = Param.String("pJ", "Default values set from integer_adder.yml")
	time_units = Param.String("ns", "Default values set from integer_adder.yml")
	area_units = Param.String("um^2", "Default values set from integer_adder.yml")
	fu_latency = Param.UInt32(5, "Default values set from integer_adder.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from integer_adder.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from integer_adder.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from integer_adder.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from integer_adder.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from integer_adder.yml")
	area = Param.UInt32(5.981433, "Default values set from integer_adder.yml")
	path_delay = Param.UInt32(1.75, "Default values set from integer_adder.yml")

class DoubleDivider(SimObject):
	# SimObject type
	type = 'DoubleDivider'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/double_divider.hh"
	#HW Params
	alias = Param.String("double_divider", "Default values set from double_divider.yml")
	stages = Param.UInt32(3, "Default values set from double_divider.yml")
	cycles = Param.UInt32(16, "Default values set from double_divider.yml")
	enum_value = Param.UInt32(10, "Default values set from double_divider.yml")
	int_size = Param.String("none", "Default values set from double_divider.yml")
	int_sign = Param.String("none", "Default values set from double_divider.yml")
	int_apmode = Param.Bool(False, "Default values set from double_divider.yml")
	fp_size = Param.String("double", "Default values set from double_divider.yml")
	fp_sign = Param.String("any", "Default values set from double_divider.yml")
	fp_apmode = Param.Bool(True, "Default values set from double_divider.yml")
	ptr_size = Param.String("none", "Default values set from double_divider.yml")
	ptr_sign = Param.String("none", "Default values set from double_divider.yml")
	ptr_apmode = Param.Bool(False, "Default values set from double_divider.yml")
	limit = Param.UInt32(0, "Default values set from double_divider.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from double_divider.yml")
	energy_units = Param.String("pJ", "Default values set from double_divider.yml")
	time_units = Param.String("ns", "Default values set from double_divider.yml")
	area_units = Param.String("um^2", "Default values set from double_divider.yml")
	fu_latency = Param.UInt32(5, "Default values set from double_divider.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from double_divider.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from double_divider.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from double_divider.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from double_divider.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from double_divider.yml")
	area = Param.UInt32(5.981433, "Default values set from double_divider.yml")
	path_delay = Param.UInt32(1.75, "Default values set from double_divider.yml")

class FloatAdder(SimObject):
	# SimObject type
	type = 'FloatAdder'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/float_adder.hh"
	#HW Params
	alias = Param.String("float_adder", "Default values set from float_adder.yml")
	stages = Param.UInt32(3, "Default values set from float_adder.yml")
	cycles = Param.UInt32(5, "Default values set from float_adder.yml")
	enum_value = Param.UInt32(5, "Default values set from float_adder.yml")
	int_size = Param.String("none", "Default values set from float_adder.yml")
	int_sign = Param.String("none", "Default values set from float_adder.yml")
	int_apmode = Param.Bool(False, "Default values set from float_adder.yml")
	fp_size = Param.String("single", "Default values set from float_adder.yml")
	fp_sign = Param.String("any", "Default values set from float_adder.yml")
	fp_apmode = Param.Bool(True, "Default values set from float_adder.yml")
	ptr_size = Param.String("none", "Default values set from float_adder.yml")
	ptr_sign = Param.String("none", "Default values set from float_adder.yml")
	ptr_apmode = Param.Bool(False, "Default values set from float_adder.yml")
	limit = Param.UInt32(0, "Default values set from float_adder.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from float_adder.yml")
	energy_units = Param.String("pJ", "Default values set from float_adder.yml")
	time_units = Param.String("ns", "Default values set from float_adder.yml")
	area_units = Param.String("um^2", "Default values set from float_adder.yml")
	fu_latency = Param.UInt32(5, "Default values set from float_adder.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from float_adder.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from float_adder.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from float_adder.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from float_adder.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from float_adder.yml")
	area = Param.UInt32(5.981433, "Default values set from float_adder.yml")
	path_delay = Param.UInt32(1.75, "Default values set from float_adder.yml")

class FloatMultiplier(SimObject):
	# SimObject type
	type = 'FloatMultiplier'
	# gem5-SALAM attached header
	cxx_header = "hwacc/HWModeling/generated/functionalunits/float_multiplier.hh"
	#HW Params
	alias = Param.String("float_multiplier", "Default values set from float_multiplier.yml")
	stages = Param.UInt32(3, "Default values set from float_multiplier.yml")
	cycles = Param.UInt32(5, "Default values set from float_multiplier.yml")
	enum_value = Param.UInt32(7, "Default values set from float_multiplier.yml")
	int_size = Param.String("none", "Default values set from float_multiplier.yml")
	int_sign = Param.String("none", "Default values set from float_multiplier.yml")
	int_apmode = Param.Bool(False, "Default values set from float_multiplier.yml")
	fp_size = Param.String("single", "Default values set from float_multiplier.yml")
	fp_sign = Param.String("any", "Default values set from float_multiplier.yml")
	fp_apmode = Param.Bool(True, "Default values set from float_multiplier.yml")
	ptr_size = Param.String("none", "Default values set from float_multiplier.yml")
	ptr_sign = Param.String("none", "Default values set from float_multiplier.yml")
	ptr_apmode = Param.Bool(False, "Default values set from float_multiplier.yml")
	limit = Param.UInt32(0, "Default values set from float_multiplier.yml")
	#Power Params
	power_units = Param.String("mW", "Default values set from float_multiplier.yml")
	energy_units = Param.String("pJ", "Default values set from float_multiplier.yml")
	time_units = Param.String("ns", "Default values set from float_multiplier.yml")
	area_units = Param.String("um^2", "Default values set from float_multiplier.yml")
	fu_latency = Param.UInt32(5, "Default values set from float_multiplier.yml")
	internal_power = Param.UInt32(0.009743773, "Default values set from float_multiplier.yml")
	switch_power = Param.UInt32(0.007400587, "Default values set from float_multiplier.yml")
	dynamic_power = Param.UInt32(0.001800732, "Default values set from float_multiplier.yml")
	dynamic_energy = Param.UInt32(0.009003937, "Default values set from float_multiplier.yml")
	leakage_power = Param.UInt32(7.395312e-05, "Default values set from float_multiplier.yml")
	area = Param.UInt32(5.981433, "Default values set from float_multiplier.yml")
	path_delay = Param.UInt32(1.75, "Default values set from float_multiplier.yml")

