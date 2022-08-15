#include "value.hh"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "sim/sim_object.hh"

SALAM::Value::Value(uint64_t id, gem5::SimObject * _owner, bool _dbg) {
    uid = id;
    size = 0;
    owner = _owner;
    dbg = _dbg;
}

SALAM::Value::~Value()
{
}

// copy constructor
SALAM::Value::Value(const Value &copy_val)
{
    uid = copy_val.uid;
    returnReg = copy_val.returnReg;
    valueTy = copy_val.valueTy;
    size = copy_val.size;
    ir_string = copy_val.ir_string;
    ir_stub = copy_val.ir_stub;
    ptr_size = copy_val.ptr_size;
    reading_value_from_map = copy_val.reading_value_from_map;
    is_unwrap = copy_val.is_unwrap;

    owner = copy_val.owner;
    dbg = copy_val.dbg;
}

SALAM::Value::Value(std::shared_ptr<SALAM::Value> copy_val)
{
    uid = copy_val->getUID();
    returnReg = copy_val->getReg();
    valueTy = copy_val->getType();
    size = copy_val->getSize();
    ir_string = copy_val->getIRString();
    ir_stub = copy_val->getIRStub();
    ptr_size = copy_val->getPtrSize();
    reading_value_from_map = copy_val.get()->reading_value_from_map;
    is_unwrap = copy_val.get()->is_unwrap;

    owner = copy_val->getOwner();
    dbg = copy_val->debug();
}

// operator equals
SALAM::Value&
SALAM::Value::operator = (Value &copy_val)
{
    uid = copy_val.uid;
    returnReg = copy_val.returnReg;
    valueTy = copy_val.valueTy;
    size = copy_val.size;
    ir_string = copy_val.ir_string;
    ir_stub = copy_val.ir_stub;
    reading_value_from_map = copy_val.reading_value_from_map;
    is_unwrap = copy_val.is_unwrap;

    return *this;
}

SALAM::Value::Value_Debugger::Value_Debugger()
{
}

void
SALAM::Value::Value_Debugger::dumper(SALAM::Value *value)
{
}

void
SALAM::Value::initialize(llvm::Value * irval, SALAM::irvmap * irmap) {
    llvm::Type *irtype = irval->getType();
    if (irtype->getTypeID() == llvm::Type::PointerTyID) {
        size = 64; //We assume a 64-bit memory address space
    } else {
        size = irtype->getScalarSizeInBits();
    }
    valueTy = irtype->getTypeID();
    // Link Return Register
    if (size>0) addRegister(irtype, true);

    std::string tmpStr1;
    llvm::raw_string_ostream ss(tmpStr1);
    ss << *irval;
    ir_string = ss.str();

    std::string tmpStr2;
    llvm::raw_string_ostream ss2(tmpStr2);
    irval->printAsOperand(ss2);
    ir_stub = ss2.str();


}

void
SALAM::Value::addRegister(llvm::Type *irtype, bool istracked) {
    if (irtype->isPointerTy()) {
        returnReg = std::make_shared<PointerRegister>(istracked);
    } else if (irtype->isIntegerTy()) {
        returnReg = std::make_shared<APIntRegister>(irtype, istracked);
    } else if (irtype->isFloatingPointTy()) {
        returnReg = std::make_shared<APFloatRegister>(irtype, istracked);
    } else {
        //assert(0); // Type is invalid for a register
        returnReg = nullptr;
    }
}

#if USE_LLVM_AP_VALUES
    void
    SALAM::Value::addAPIntRegister(const llvm::APInt & val) {

        assert(irtype->isIntegerTy());
        returnReg = std::make_shared<APIntRegister>(val);
    }
    void
    SALAM::Value::addAPIntRegister(const llvm::APSInt & val) {

        assert(irtype->isIntegerTy());
        returnReg = std::make_shared<APIntRegister>(val);
    }
    void
    SALAM::Value::addAPFloatRegister(const llvm::APFloat & val) {

        assert(irtype->isFloatingPointTy());
        returnReg = std::make_shared<APFloatRegister>(val);
    }
#else
    void
    SALAM::Value::addAPIntRegister(const uint64_t & val) {

        assert(valueTy == llvm::Type::IntegerTyID);
        uint64_t bitmask = 0;
        assert((size <= 64) &&
            "Only 64-bit and smaller values are \
             supported when not using AP values.");
        bitmask = (bitmask - 1) >> (64 - size);
        returnReg = std::make_shared<APIntRegister>(val & bitmask);
    }
    void
    SALAM::Value::addAPFloatRegister(const uint64_t & val) {

        assert((valueTy == llvm::Type::FloatTyID) ||
                valueTy == llvm::Type::DoubleTyID);
        uint64_t bitmask = 0;
        assert((size <= 64) &&
            "Only 64-bit and smaller values are \
            supported when not using AP values.");
        bitmask = (bitmask - 1) >> (64 - size);
        returnReg = std::make_shared<APFloatRegister>(val & bitmask);
    }
#endif

void
SALAM::Value::addPointerRegister(bool istracked, bool isnull) {
    // assert(valueTy == llvm::Type::PointerTyID);
    returnReg = std::make_shared<PointerRegister>(istracked, isnull);
}
void
SALAM::Value::addPointerRegister(uint64_t val, bool istracked, bool isnull) {
    assert(valueTy == llvm::Type::PointerTyID);
    returnReg = std::make_shared<PointerRegister>(val, istracked, isnull);
}

#if USE_LLVM_AP_VALUES
    void
    SALAM::Value::setRegisterValue(const llvm::APInt &data) {

        if (dbg) DPRINTFS(Runtime, owner, "| APInt Register\n");
        if (returnReg->isInt()) {
            returnReg->writeIntData(data);
        } else {
            if (dbg) DPRINTFS(Runtime, owner, "Unsupported type for register operation. \
                Tried to place integer data in non-integer register.\n");
        }
    }
    void
    SALAM::Value::setRegisterValue(const llvm::APFloat &data) {

        if (dbg) DPRINTFS(Runtime, owner, "| APFloat Register\n");
        if (returnReg->isFP()) {
            returnReg->writeFloatData(data);
        } else {
            if (dbg) DPRINTFS(Runtime, owner, "Unsupported type for register operation. \
                Tried to place float data in non-float register.\n");
        }
    }
#endif

void
SALAM::Value::setRegisterValue(const uint64_t data) {

    if (returnReg->isPtr()) {
        // std::cerr << getUID() << " : " << getIRString() << ": setting pointer register value " << std::hex << data  << std::endl;
        returnReg->writePtrData(data, ptr_size);
        // std::cerr << "After write 1: " <<  std::dec << std::endl;

        if (dbg) DPRINTFS(Runtime, owner, "| Ptr Register\n");
        returnReg->writePtrData(data);
        // std::cerr << "After write 2: " <<  std::dec << std::endl;

    } else {
    #if USE_LLVM_AP_VALUES
        if (dbg) DPRINTFS(Runtime, owner, "Unsupported type for register operation. \
            Tried to place Ptr data in non-Ptr register.\n");
    #else
        if (returnReg->isInt()) {
            if (dbg) DPRINTFS(Runtime, owner, "| Int Register\n");
            returnReg->writeIntData(data, getSizeInBytes());
        } else {
            if (dbg) DPRINTFS(Runtime, owner, "| FP Register\n");
            returnReg->writeFloatData(data, getSizeInBytes());
        }
    #endif
    }
}
void
SALAM::Value::setRegisterValue(uint8_t * data) {
    if (dbg) DPRINTFS(Runtime, owner, "| Set Register Data - ");
    switch (valueTy) {
    #if USE_LLVM_AP_VALUES
        case llvm::Type::FloatTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Float\n");
            float tmpData;
            std::memcpy(&tmpData, data, sizeof(float));
            setRegisterValue(llvm::APFloat(tmpData));
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Double\n");
            double tmpData;
            std::memcpy(&tmpData, data, sizeof(double));
            setRegisterValue(llvm::APFloat(tmpData));
            break;
        }
        case llvm::Type::IntegerTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Integer Type | Size = %d\n", size);
            if (size > 64) {
                size_t bigIntLen = ((size - 1) / 64) + 1;
                setRegisterValue(llvm::APInt(size,
                    llvm::ArrayRef<uint64_t>((uint64_t *)data, bigIntLen)));
            } else {
                setRegisterValue(llvm::APInt(size, *(uint64_t *)(data)));
            }
            break;
        }
    #else
        case llvm::Type::FloatTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Float\n");
            returnReg->writeFloatData(*(uint64_t *)data, (size_t)4);
            break;
        }
        case llvm::Type::DoubleTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Double\n");
            returnReg->writeFloatData(*(uint64_t *)data, (size_t)8);
            break;
        }
        case llvm::Type::IntegerTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Integer Type | Size = %d\n", size);
            returnReg->writeIntData(*(uint64_t *)data,
                                    (size_t)getSizeInBytes());
            break;
        }
    #endif
        case llvm::Type::PointerTyID:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Pointer\n");
            returnReg->writePtrData(*(uint64_t *)data, ptr_size);
            break;
        }
        default:
        {
            if (dbg) DPRINTFS(Runtime, owner, "Unsupported type for register operation\n");
            assert(0);
        }
    }
}

void
SALAM::Value::setRegisterValue(bool data) {
    if (dbg) DPRINTFS(Runtime, owner, "| Int Register\n");
    if (returnReg->isInt()) {
    #if USE_LLVM_AP_VALUES
        if (data) {
            setRegisterValue(llvm::APInt::getAllOnesValue(1));
        } else {
            setRegisterValue(llvm::APInt::getNullValue(1));
        }
    #else
        if (data) {
            setRegisterValue((uint64_t)1);
        } else {
            setRegisterValue((uint64_t)0);
        }
    #endif
    } else {
        if (dbg) DPRINTFS(Runtime, owner, "Unsupported type for register operation. \
            Tried to place integer data in non-integer register.\n");
    }
}

void
SALAM::Value::setRegisterValue(std::shared_ptr<SALAM::Register> reg) {
    if (reg->isPtr()) {
        setRegisterValue((reg->getPtrData()));
    } else if (reg->isFP()) {
        setRegisterValue((reg->getFloatData()));
    } else {
        setRegisterValue((reg->getIntData()));
    }
    if (dbg) DPRINTFS(Runtime, owner, "||==setRegisterValue====\n");
}