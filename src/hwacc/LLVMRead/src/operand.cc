#include "operand.hh"


SALAM::Constant::Constant(uint64_t id) :
                          Value(id)
{
}

void
SALAM::Constant::initialize(llvm::Value * irval,
                            SALAM::irvmap * irmap,
                            SALAM::valueListTy * values)
{
    //Initialize SALAM::Value
    // if (llvm::isa<llvm::Function>(irval)) {
    //     addPointerRegister(false, true);
    //     return;
    // }
    SALAM::Value::initialize(irval, irmap);
    // Parse the constant value
    llvm::ConstantData * cd = llvm::dyn_cast<llvm::ConstantData>(irval);
    llvm::ConstantExpr * ce = llvm::dyn_cast<llvm::ConstantExpr>(irval);
    llvm::Type *irtype = irval->getType();
    if (cd) {
        // The constant is a llvm::ConstantData.
        // Get it's value and store it in constValue
        if (irtype->isFloatingPointTy()) {
            llvm::ConstantFP * fp = llvm::dyn_cast<llvm::ConstantFP>(cd);
        #if USE_LLVM_AP_VALUES
            addAPFloatRegister(fp->getValueAPF());
        #else
            auto apfp = fp->getValueAPF();
            auto api = apfp.bitcastToAPInt();
            addAPFloatRegister(api.getLimitedValue());
        #endif
        } else if (irtype->isIntegerTy()) {
            llvm::ConstantInt * in = llvm::dyn_cast<llvm::ConstantInt>(cd);
        #if USE_LLVM_AP_VALUES
            addAPIntRegister(in->getValue());
        #else
            auto api = in->getValue();
            addAPIntRegister(api.getLimitedValue());
        #endif
        } else if (irtype->isPointerTy()) {
            assert(llvm::dyn_cast<llvm::ConstantPointerNull>(cd));
            addPointerRegister(false, true);
        }
    } else if (ce) {
        // The constant is an expression. We need to parse the expression
        for (auto op : ce->operand_values()) {
            // Iterate over operands and add
            // new values to our map and value list
            auto mapit = irmap->find(op);
            if (mapit == irmap->end()) {
                uint64_t id = values->back()->getUID() + 1;
                std::shared_ptr<SALAM::Constant> con =
                    std::make_shared<SALAM::Constant>(id);
                values->push_back(con);
                irmap->insert(SALAM::irvmaptype(op, con));
                operands.push_back(con);
                con->initialize(op, irmap, values);
            } else {
                std::shared_ptr<SALAM::Value> opval = mapit->second;
                operands.push_back(opval);
            }
        }

    #if USE_LLVM_AP_VALUES
        #if (LLVM_VERSION_MAJOR <= 9)
            auto rounding = llvm::APFloat::roundingMode::rmNearestTiesToEven;
        #else
            auto rounding = llvm::APFloat::roundingMode::NearestTiesToEven;
        #endif
    #endif

        switch(ce->getOpcode()) {
            case llvm::Instruction::Trunc:
            {
                auto opdata = operands.front()->getIntRegValue();
            #if USE_LLVM_AP_VALUES
                addAPIntRegister(opdata.trunc(size));
            #else
                addAPIntRegister(opdata);
            #endif
                break;
            }
            case llvm::Instruction::ZExt:
            {
                auto opdata = operands.front()->getIntRegValue();
            #if USE_LLVM_AP_VALUES
                opdata.setIsSigned(false);
                addAPIntRegister(opdata.extend(size));
            #else
                addAPIntRegister(opdata);
            #endif
                break;
            }
            case llvm::Instruction::SExt:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getIntRegValue();
                opdata.setIsSigned(true);
                addAPIntRegister(opdata.extend(size));
            #else
                int64_t tmp = operands.front()->getSIntRegValue();
                addAPIntRegister((uint64_t)tmp);
            #endif
                break;
            }
            case llvm::Instruction::FPToUI:
            {
            #if USE_LLVM_AP_VALUES
                llvm::APSInt tmp(size, true);
                bool exact;
                auto opdata = operands.front()->getFloatRegValue();
                auto err = opdata.convertToInteger(tmp,
                                                  rounding,
                                                  &exact);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPIntRegister(tmp);
            #else
                if (operands.front()->getSize() == 32) {
                    auto opdata = operands.front()->getFloatFromReg();
                    addAPIntRegister((uint64_t)opdata);
                } else {
                    auto opdata = operands.front()->getDoubleFromReg();
                    addAPIntRegister((uint64_t)opdata);
                }
            #endif
                break;
            }
            case llvm::Instruction::FPToSI:
            {
            #if USE_LLVM_AP_VALUES
                llvm::APSInt tmp(size, false);
                bool exact;
                auto opdata = operands.front()->getFloatRegValue();
                auto err = opdata.convertToInteger(tmp,
                                                  rounding,
                                                  &exact);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPIntRegister(tmp);
            #else
                if (operands.front()->getSize() == 32) {
                    auto opdata = operands.front()->getFloatFromReg();
                    addAPIntRegister((uint64_t)(int64_t)opdata);
                } else {
                    auto opdata = operands.front()->getDoubleFromReg();
                    addAPIntRegister((uint64_t)(int64_t)opdata);
                }
            #endif
                break;
            }
            case llvm::Instruction::UIToFP:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getIntRegValue();
                llvm::APFloat tmp(irtype->getFltSemantics());
                auto err = tmp.convertFromAPInt(opdata, false, rounding);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                auto opdata = operands.front()->getUIntRegValue();
                switch (size) {
                    case 32:
                    {
                        float tmp = (float)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    case 64:
                    {
                        double tmp = (double)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                        break;
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::SIToFP:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getIntRegValue();
                llvm::APFloat tmp(irtype->getFltSemantics());
                auto err = tmp.convertFromAPInt(opdata, false, rounding);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                auto opdata = operands.front()->getSIntRegValue();
                switch (size) {
                    case 32:
                    {
                        float tmp = (float)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    case 64:
                    {
                        double tmp = (double)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                        break;
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::FPTrunc:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getFloatRegValue();
                llvm::APFloat tmp(opdata);
                bool losesInfo;
                auto err = tmp.convert(irtype->getFltSemantics(),
                                       rounding, &losesInfo);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                switch (operands.front()->getSize()) {
                    case 64:
                    {
                        double opdata = operands.front()->getDoubleFromReg();
                        float tmp = (float)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::FPExt:
            {
            #if USE_LLVM_AP_VALUES
                auto opdata = operands.front()->getFloatRegValue();
                llvm::APFloat tmp(opdata);
                bool losesInfo;
                auto err = tmp.convert(irtype->getFltSemantics(),
                                       rounding, &losesInfo);
                assert(err == llvm::APFloatBase::opStatus::opOK);
                addAPFloatRegister(tmp);
            #else
                switch (operands.front()->getSize()) {
                    case 32:
                    {
                        float opdata = operands.front()->getFloatFromReg();
                        double tmp = (double)opdata;
                        addAPFloatRegister(*(uint64_t *)&tmp);
                        break;
                    }
                    default:
                    {
                        assert(0 &&
                            "Must use AP values for nonstandard FP sizes.");
                    }
                }
            #endif
                break;
            }
            case llvm::Instruction::PtrToInt:
            {
                auto opdata = operands.front()->getReg()->getPtrData();
            #if USE_LLVM_AP_VALUES
                addAPIntRegister(llvm::APInt(64, opdata));
            #else
                addAPIntRegister(opdata);
            #endif
                break;
            }
            case llvm::Instruction::IntToPtr:
            {
                auto opdata = operands.front()->getIntRegValue();
            #if USE_LLVM_AP_VALUES
                assert(opdata.isUnsigned());
                int64_t tmp = opdata.getExtValue();
                addPointerRegister(*(uint64_t *)&tmp, false, false);
            #else
                addPointerRegister(opdata, false, false);
            #endif
                break;
            }
            default: {
                llvm::errs() << "Initializing nested Constant failed" << *irval << "\n";

                assert(0); // We do not support this nested ConstantExpr
            }
        }
    } else {
        llvm::errs() << "Initializing Constant failed " << *irval << "\n";
        assert(0); // The value is not a supported type of llvm::Constant
    }
}

SALAM::GlobalConstant::GlobalConstant(uint64_t id) :
                                      Constant(id)
{
}

void
SALAM::GlobalConstant::initialize(llvm::Value * irval,
                                  SALAM::irvmap * irmap,
                                  SALAM::valueListTy * values)
{
    // Parse the initializer of the value
    auto glb = llvm::dyn_cast<llvm::GlobalVariable>(irval);
    assert(glb);
    llvm::Constant *initializer;
    // assert(glb->hasInitializer());
    if (!glb->hasInitializer())
        initializer = llvm::ConstantInt::get(glb->getContext(), llvm::APInt(/*nbits*/32, 0, /*bool*/false));

    else
        initializer = glb->getInitializer();
    // glb->getInitializer()->print(llvm::outs());

    //Initialize SALAM::ConstantData
    // SALAM::Constant::initialize(glb->getInitializer(), irmap, values);
    SALAM::Constant::initialize(initializer, irmap, values);

}

SALAM::Argument::Argument(uint64_t id) :
                          Value(id)
{
}

void
SALAM::Argument::initialize(llvm::Value * irval, SALAM::irvmap * irmap)
{
    //Initialize SALAM::Value
    SALAM::Value::initialize(irval, irmap);
    addRegister(irval->getType());
}

SALAM::Operand::Operand_Debugger::Operand_Debugger()
{

}

void
SALAM::Operand::Operand_Debugger::dumper(Operand * op)
{

}

// copy constructor
SALAM::Operand::Operand(const SALAM::Operand &copy_val):
           SALAM::Value(copy_val)
{
    lockedValue = copy_val.lockedValue;
    set = copy_val.set;
}

// copy constructor from base
SALAM::Operand::Operand(const SALAM::Value &copy_val):
           SALAM::Value(copy_val)
{ // Update here for values in the copied value base class
    initOperandReg();
}

SALAM::Operand::Operand(std::shared_ptr<SALAM::Value> copy_val):
            SALAM::Value(copy_val)
{
    initOperandReg();
}

// operator equals
SALAM::Operand&
SALAM::Operand::operator = (SALAM::Operand &copy_val)
{
    uid = copy_val.uid;
      returnReg = copy_val.returnReg;
    valueTy = copy_val.valueTy;
    size = copy_val.size;
    lockedValue = copy_val.lockedValue;
    set = copy_val.set;
    return *this;
}

void
SALAM::Operand::initOperandReg()
{
    bool istracked = false;
    if (returnReg->isPtr()) {
        DPRINTF(Runtime, "Operand Ptr Register Initialized\n");
        lockedValue = std::make_shared<PointerRegister>(istracked);
    } else if (returnReg->isInt()) {
        DPRINTF(Runtime, "Operand Int Register Initialized\n");
        lockedValue = std::make_shared<APIntRegister>(size, istracked);
    } else if (returnReg->isFP()) {
        DPRINTF(Runtime, "Operand FP Register Initialized\n");
        lockedValue =
            std::make_shared<APFloatRegister>(valueTy, istracked);
    } else {
        DPRINTF(Runtime, "Invalid register type. Dumping Operand details\n");
        dump();
        assert(0); // Type is invalid for a register
    }
}

void
SALAM::Operand::initialize(llvm::Value * irval, SALAM::irvmap * irmap)
{
    SALAM::Value::initialize(irval, irmap);
}

void
SALAM::Operand::updateOperandRegister() {
    assert(lockedValue);
    if (lockedValue->isPtr()) {
        lockedValue->writePtrData(returnReg->getPtrData(true),
                                  getSizeInBytes());
    } else if (lockedValue->isInt()) {
        lockedValue->writeIntData(returnReg->getIntData(true));
    } else if (lockedValue->isFP()) {
        lockedValue->writeFloatData(returnReg->getFloatData(true));
    }
}
