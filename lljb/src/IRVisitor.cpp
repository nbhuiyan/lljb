#include "lljb/IRVisitor.hpp"

#include "ilgen/TypeDictionary.hpp"
#include "ilgen/BytecodeBuilder.hpp"

#include <cassert>
#include <vector>

namespace lljb {

void IRVisitor::visitInstruction(llvm::Instruction &I){
    llvm::outs() << "Unimplemented instruction being visited: " << I << "\n";
    assert(0 && "Unimplemented instruction!");
}

void IRVisitor::visitReturnInst(llvm::ReturnInst &I){
    llvm::outs() << "return inst: " << I << "\n";
    if (!I.getNumOperands()){
        _builder->Return();
    }
    else {
        llvm::Value * value = I.getOperand(0);
        TR::IlValue * ilValue = getIlValue(value);
        _builder->Return(ilValue);
    }
}

void IRVisitor::visitAllocaInst(llvm::AllocaInst &I){
    llvm::outs() << "alloca inst: " << I << " allocated type: " << *I.getAllocatedType() << "\n";
    TR::IlValue * ilValue = nullptr;
    if (I.getAllocatedType()->isStructTy()){
        ilValue = _builder->CreateLocalStruct(_methodBuilder->getIlType(_td,I.getAllocatedType()));
    }
    else if (I.getAllocatedType()->isArrayTy()){
        ilValue = _builder->CreateLocalArray(
                            I.getAllocatedType()->getArrayNumElements(),
                            _methodBuilder->getIlType(_td,I.getAllocatedType()->getArrayElementType()));
    }
    else{
    ilValue =
        _builder->CreateLocalArray(
                            1,
                            _methodBuilder->getIlType(_td,I.getAllocatedType()));
    }
    _methodBuilder->mapIRtoIlValue(&I, ilValue);
}

void IRVisitor::visitLoadInst(llvm::LoadInst &I){
    llvm::outs() << "load inst: " << I << "\n";
    llvm::Value * source = I.getPointerOperand();
    TR::IlValue * ilSrc = getIlValue(source);
    TR::IlValue * loadedVal = _builder->LoadAt(
                                _td->PointerTo(_methodBuilder->getIlType(_td,I.getType())),
                                _builder->
                                IndexAt(
                                    _td->PointerTo(_methodBuilder->getIlType(_td,I.getType())),
                                    ilSrc,
                                    _builder->ConstInt32(0)));
    _methodBuilder->mapIRtoIlValue(&I, loadedVal);
}

void IRVisitor::visitStoreInst(llvm::StoreInst &I){
    llvm::outs() << "store inst: " << I << "\n";
    llvm::Value * dest = I.getPointerOperand();
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = getIlValue(value);

    _builder->StoreAt(
                _builder->IndexAt(
                    _td->PointerTo(_methodBuilder->getIlType(_td,value->getType())),
                    _methodBuilder->getIlValue(dest),
                    _builder->ConstInt32(0)),
                ilValue);
}

void IRVisitor::visitBinaryOperator(llvm::BinaryOperator &I){
    llvm::outs() << "binaryOp inst: " << I << "\n";
    TR::IlValue * lhs = getIlValue(I.getOperand(0));
    TR::IlValue * rhs = getIlValue(I.getOperand(1));
    TR::IlValue * result = nullptr;
    switch (I.getOpcode()){
        case llvm::BinaryOperator::Add:
        case llvm::BinaryOperator::FAdd:
            result = _builder->Add(lhs,rhs);
            break;
        case llvm::BinaryOperator::Sub:
        case llvm::BinaryOperator::FSub:
            result = _builder->Sub(lhs, rhs);
            break;
        case llvm::BinaryOperator::Mul:
        case llvm::BinaryOperator::FMul:
            result = _builder->Mul(lhs, rhs);
            break;
        case llvm::BinaryOperator::SDiv:
        case llvm::BinaryOperator::FDiv:
        case llvm::BinaryOperator::UDiv:
            result = _builder->Div(lhs, rhs);
            break;
        default:
            assert(0 && "Unknown binary operand");
            break;
    }
    _methodBuilder->mapIRtoIlValue(&I, result);
}

void IRVisitor::visitCmpInst(llvm::CmpInst &I){
    llvm::outs() << "cmp inst: " << I << "\n";
    TR::IlValue * lhs = getIlValue(I.getOperand(0));
    TR::IlValue * rhs = getIlValue(I.getOperand(1));

    TR::IlValue * result = nullptr;

    switch (I.getPredicate()){
        case llvm::CmpInst::Predicate::ICMP_EQ:
        case llvm::CmpInst::Predicate::FCMP_OEQ:
        case llvm::CmpInst::Predicate::FCMP_UEQ:
            result = _builder->EqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_NE:
        case llvm::CmpInst::Predicate::FCMP_ONE:
        case llvm::CmpInst::Predicate::FCMP_UNE:
            result = _builder->NotEqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_UGT:
            result = _builder->UnsignedGreaterThan(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_UGE:
            result = _builder->UnsignedGreaterOrEqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_ULT:
            result = _builder->UnsignedLessThan(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_ULE:
            result = _builder->UnsignedLessOrEqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SGT:
        case llvm::CmpInst::Predicate::FCMP_OGT:
        case llvm::CmpInst::Predicate::FCMP_UGT:
            result = _builder->GreaterThan(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SGE:
        case llvm::CmpInst::Predicate::FCMP_OGE:
        case llvm::CmpInst::Predicate::FCMP_UGE:
            result = _builder->GreaterOrEqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SLT:
        case llvm::CmpInst::Predicate::FCMP_OLT:
        case llvm::CmpInst::Predicate::FCMP_ULT:
            result = _builder->LessThan(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SLE:
        case llvm::CmpInst::Predicate::FCMP_OLE:
        case llvm::CmpInst::Predicate::FCMP_ULE:
            result = _builder->LessOrEqualTo(lhs, rhs);
            break;
        default:
            assert(0 && "Unknown CmpInst predicate");
            break;
    }
    _methodBuilder->mapIRtoIlValue(&I, result);
}

void IRVisitor::visitBranchInst(llvm::BranchInst &I){
    llvm::outs() << "Branch inst: " << I /*<< "dest: " << *(I.getOperand(0)) */<< "\n";
    if (I.isUnconditional()){
        llvm::Value * dest = I.getSuccessor(0);
        TR::BytecodeBuilder * destBuilder = _methodBuilder->getByteCodeBuilder(dest);
        assert(destBuilder && "failed to find builder for target basic block");
        _builder->Goto(destBuilder);
        _builder->AddSuccessorBuilder(&destBuilder);
    }
    else{
        TR::IlValue * condition = _methodBuilder->getIlValue(I.getCondition());
        TR::IlBuilder * ifTrue = _methodBuilder->getByteCodeBuilder(I.getSuccessor(0));
        TR::IlBuilder * ifFalse = _methodBuilder->getByteCodeBuilder(I.getSuccessor(1));
        assert(ifTrue && ifFalse && condition && "Failed to find destination blocks for ifThenElse");
        _builder->IfThenElse(&ifTrue, &ifFalse, condition);
    }
}

void IRVisitor::visitCallInst(llvm::CallInst &I){
    llvm::outs() << "call inst: " << I << "\n";
    TR::IlValue * result = nullptr;
    llvm::Function * callee = I.getCalledFunction();
    const char * calleeName = callee->getName().data();
    _methodBuilder->defineFunction(callee);
    std::size_t numArgs = callee->arg_size();
    if (!numArgs){
        result = _builder->Call(calleeName, numArgs);
    }
    else {
        std::vector<TR::IlValue *> params;
        for (int i = 0; i < numArgs; i++){
            TR::IlValue * parameter = getIlValue(I.getArgOperand(i));
            params.push_back(parameter);
        }
        result = _builder->Call(calleeName, numArgs, params.data());
    }
    _methodBuilder->mapIRtoIlValue(&I, result);
}

void IRVisitor::visitCastInst(llvm::CastInst &I){
    llvm::outs() << "Cast inst: " << I << "\n";
    TR::IlValue * srcVal = getIlValue(I.getOperand(0));
    TR::IlType * toIlType = _methodBuilder->getIlType(_td,I.getDestTy());
    TR::IlValue * result = _builder->ConvertTo(toIlType, srcVal);
    _methodBuilder->mapIRtoIlValue(&I, result);
}

void IRVisitor::visitGetElementPtrInst(llvm::GetElementPtrInst &I){
    llvm::outs() << "getElementPtrInst: " << I << "\n";
    TR::IlValue * ilValue = nullptr;
    if (I.getSourceElementType()->isStructTy()){
        llvm::ConstantInt * indextConstantInt = llvm::dyn_cast<llvm::ConstantInt>(I.getOperand(2));
        unsigned elementIndex = indextConstantInt->getZExtValue();
        ilValue = _builder->StructFieldInstanceAddress(I.getSourceElementType()->getStructName().data(),
                                                                _methodBuilder->getMemberNameFromIndex(elementIndex),
                                                                getIlValue(I.getOperand(0)));
    }
    else if (I.getSourceElementType()->isArrayTy()){
        ilValue =
            _builder->IndexAt(
                _td->PointerTo(
                    _methodBuilder->getIlType(_td,I.getSourceElementType()->getArrayElementType())),
                getIlValue(I.getOperand(0)),
                getIlValue(I.getOperand(2)));
    }
    else {
        assert(0 && "unknown source type for getElementPtr Instruction");
    }

    _methodBuilder->mapIRtoIlValue(&I, ilValue);
}

TR::IlValue * IRVisitor::createConstIntIlValue(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::ConstantInt * constInt = llvm::dyn_cast<llvm::ConstantInt>(value);
    int64_t signExtendedValue = constInt->getSExtValue();
    if (constInt->getBitWidth() <= 8) ilValue = _builder->ConstInt8(signExtendedValue);
    else if (constInt->getBitWidth() <= 16) ilValue = _builder->ConstInt16(signExtendedValue);
    else if (constInt->getBitWidth() <= 32) ilValue = _builder->ConstInt32(signExtendedValue);
    else if (constInt->getBitWidth() <= 64) ilValue = _builder->ConstInt64(signExtendedValue);

    return ilValue;
}

TR::IlValue * IRVisitor::createConstFPIlValue(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::ConstantFP * constFP = llvm::dyn_cast<llvm::ConstantFP>(value);
    llvm::APFloat apf = constFP->getValueAPF();
    if (apf.getSizeInBits(apf.getSemantics()) <= 32){
        float rawValue = apf.convertToFloat();
        ilValue = _builder->ConstFloat(rawValue);
    }
    else {
        double rawValue = apf.convertToDouble();
        ilValue = _builder->ConstDouble(rawValue);
    }

    return ilValue;
}

TR::IlValue * IRVisitor::loadParameter(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::Argument * arg = llvm::dyn_cast<llvm::Argument>(value);
    ilValue = _builder->Load(_methodBuilder->getParamNameFromIndex(arg->getArgNo()));
    return ilValue;
}

TR::IlValue * IRVisitor::loadGlobal(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::GlobalVariable * globalVar = llvm::dyn_cast<llvm::GlobalVariable>(value);
    ilValue = getIlValue(globalVar->getInitializer());
    return ilValue;
}

TR::IlValue * IRVisitor::createConstExprIlValue(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::ConstantExpr * constExpr = llvm::dyn_cast<llvm::ConstantExpr>(value);
    //if (constExpr->getNumOperands() > 1) assert (0 && "unhandled const expr");
    ilValue = getIlValue(constExpr->getOperand(0));
    return ilValue;
}

TR::IlValue * IRVisitor::createConstantDataArrayVal(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::ConstantDataArray * constDataArray = llvm::dyn_cast<llvm::ConstantDataArray>(value);
    ilValue = _builder->ConstString(constDataArray->getAsCString().data());
    return ilValue;
}

TR::IlValue * IRVisitor::getIlValue(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    switch (value->getValueID()){
        /* Constants */
        case llvm::Value::ValueTy::ConstantExprVal:
            ilValue = createConstExprIlValue(value);
            break;
        case llvm::Value::ValueTy::GlobalVariableVal:
            ilValue = loadGlobal(value);
            break;
        case llvm::Value::ValueTy::FunctionVal:
        case llvm::Value::ValueTy::GlobalAliasVal:
        case llvm::Value::ValueTy::GlobalIFuncVal:
        case llvm::Value::ValueTy::BlockAddressVal:
            assert(0 && "Unsupported constant value type");
            break;

        /* Constant data value types */
        case llvm::Value::ValueTy::ConstantIntVal:
            ilValue = createConstIntIlValue(value);
            break;
        case llvm::Value::ValueTy::ConstantFPVal:
            ilValue = createConstFPIlValue(value);
            break;
        case llvm::Value::ValueTy::ConstantDataArrayVal:
            ilValue = createConstantDataArrayVal(value);
            break;
        case llvm::Value::ValueTy::ConstantAggregateZeroVal:
        case llvm::Value::ValueTy::ConstantDataVectorVal:
        case llvm::Value::ValueTy::ConstantPointerNullVal:
        case llvm::Value::ValueTy::ConstantTokenNoneVal:
            assert(0 && "Unsupported constant data value type");
            break;

        /* Constant aggregate value types */
        case llvm::Value::ValueTy::ConstantStructVal:
            //break; //todo uncomment when done implementing
        case llvm::Value::ValueTy::ConstantArrayVal:
        case llvm::Value::ValueTy::ConstantVectorVal:
            assert(0 && "Unsupported constant aggregate value type");
            break;

        /* Other value types */
        case llvm::Value::ValueTy::ArgumentVal:
            ilValue = loadParameter(value);
            break;
        case llvm::Value::ValueTy::BasicBlockVal:
            assert(0 && "Basicblock should not be looked up this way");
            break;

        default:
            ilValue = _methodBuilder->getIlValue(value);
            break;
    }
    assert (ilValue && "failed to retrieve ilValue from llvm Value!");
    return ilValue;
}

} /* namespace lljb */
