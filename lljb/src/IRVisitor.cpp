#include "lljb/IRVisitor.hpp"

#include "ilgen/TypeDictionary.hpp"
#include "ilgen/BytecodeBuilder.hpp"

#include <cassert>

namespace lljb {

void IRVisitor::visitInstruction(llvm::Instruction &I){
    llvm::outs() << "Unimplemented instruction being visited: " << I << "\n";
    assert(0 && "Unimplemented instruction!");
}

void IRVisitor::visitReturnInst(llvm::ReturnInst &I){
    llvm::outs() << "return inst: " << I << "\n";
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = nullptr; //_methodBuilder->getIlValue(value);

    if (value->getValueID() == llvm::Value::ConstantIntVal)
        ilValue = createConstIntIlValue(value);
    else ilValue = _methodBuilder->getIlValue(value);

    _builder->Return(ilValue);
}

void IRVisitor::visitAllocaInst(llvm::AllocaInst &I){
    llvm::outs() << "alloca inst: " << I << "\n";
    TR::IlValue * ilValue = _builder->CreateLocalArray(1,_td->Int32);//_builder->NewValue(_builder->Int32);// ;
    _methodBuilder->mapIRtoIlValue(&I, ilValue);
}

void IRVisitor::visitLoadInst(llvm::LoadInst &I){
    llvm::outs() << "load inst: " << I << "\n";
    llvm::Value * source = I.getPointerOperand();
    TR::IlValue * ilSrc = _methodBuilder->getIlValue(source);
    TR::IlValue * ilDest = _builder->LoadAt(
                                _td->pInt32,
                                _builder->
                                IndexAt(
                                    _td->pInt32,
                                    ilSrc,
                                    _builder->ConstInt32(0)));
    _methodBuilder->mapIRtoIlValue(&I, ilDest);
}

void IRVisitor::visitStoreInst(llvm::StoreInst &I){
    llvm::outs() << "store inst: " << I << "\n";
    llvm::Value * dest = I.getPointerOperand();
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = nullptr;
    if (value->getValueID() == llvm::Value::ConstantIntVal)
        ilValue = createConstIntIlValue(value);
    else ilValue = _methodBuilder->getIlValue(value);

    _builder->StoreAt(
                _builder->IndexAt(
                    _td->pInt32,
                    _methodBuilder->getIlValue(dest),
                    _builder->ConstInt32(0)),
                ilValue);
}

void IRVisitor::visitBinaryOperator(llvm::BinaryOperator &I){
    llvm::outs() << "binaryOp inst: " << I << "\n";
    TR::IlValue * lhs = _methodBuilder->getIlValue(I.getOperand(0));
    TR::IlValue * rhs = _methodBuilder->getIlValue(I.getOperand(1));
    if (!lhs) lhs = createConstIntIlValue(I.getOperand(0));
    if (!rhs) rhs = createConstIntIlValue(I.getOperand(1));
    TR::IlValue * result = nullptr;
    switch (I.getOpcode()){
        case llvm::BinaryOperator::Add:
            result = _builder->Add(lhs,rhs);
            break;
        case llvm::BinaryOperator::Sub:
            result = _builder->Sub(lhs, rhs);
            break;
        case llvm::BinaryOperator::Mul:
            result = _builder->Mul(lhs, rhs);
            break;
        case llvm::BinaryOperator::SDiv:
            result = _builder->Div(lhs, rhs);
            break;
        default:
            assert(0 && "Unknown binary operand");
            break;
    }
    _methodBuilder->mapIRtoIlValue(&I, result);
}

void IRVisitor::visitICmpInst(llvm::ICmpInst &I){
    llvm::outs() << "icmp inst: " << I << "\n";
    TR::IlValue * lhs = _methodBuilder->getIlValue(I.getOperand(0));
    TR::IlValue * rhs = _methodBuilder->getIlValue(I.getOperand(1));

    if (!lhs) lhs = createConstIntIlValue(I.getOperand(0));
    if (!rhs) rhs = createConstIntIlValue(I.getOperand(1));

    TR::IlValue * result = nullptr;

    switch (I.getPredicate()){
        case llvm::CmpInst::Predicate::ICMP_EQ:
            result = _builder->EqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_NE:
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
            result = _builder->GreaterThan(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SGE:
            result = _builder->GreaterOrEqualTo(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SLT:
            result = _builder->LessThan(lhs, rhs);
            break;
        case llvm::CmpInst::Predicate::ICMP_SLE:
            result = _builder->LessOrEqualTo(lhs, rhs);
            break;
        default:
            assert(0 && "Unknown Icmp predicate");
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

} /* namespace lljb */
