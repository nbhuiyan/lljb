#include "lljb/IRVisitor.hpp"

#include <cassert>

namespace lljb {

void IRVisitor::visitInstruction(llvm::Instruction &I){
    llvm::outs() << "Unimplemented instruction being visited: " << I << "\n";
    assert(0 && "Unimplemented instruction!");
}

void IRVisitor::visitReturnInst(llvm::ReturnInst &I){
    llvm::outs() << "return inst: " << I << "\n";
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = _methodBuilder->getIlValue(value);

    if (!ilValue) ilValue = createConstIntIlValue(value);

    _builder->Return(ilValue);
}

void IRVisitor::visitAllocaInst(llvm::AllocaInst &I){
    llvm::outs() << "alloca inst: " << I << "\n";
    _methodBuilder->mapIRtoIlValue(&I, nullptr);
}

void IRVisitor::visitLoadInst(llvm::LoadInst &I){
    llvm::outs() << "load inst: " << I << "\n";
    llvm::Value * source = I.getPointerOperand();
    TR::IlValue * ilValue = _methodBuilder->getIlValue(source);
    _methodBuilder->mapIRtoIlValue(&I, ilValue);
}

void IRVisitor::visitStoreInst(llvm::StoreInst &I){
    llvm::outs() << "store inst: " << I << "\n";
    llvm::Value * dest = I.getPointerOperand();
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = _methodBuilder->getIlValue(value);

    if (!ilValue) ilValue = createConstIntIlValue(value);

    _methodBuilder->mapIRtoIlValue(dest, ilValue);
}

void IRVisitor::visitBinaryOperator(llvm::BinaryOperator &I){
    llvm::outs() << "binaryOp inst: " << I << "\n";
    TR::IlValue * lhs = _methodBuilder->getIlValue(I.getOperand(0));
    TR::IlValue * rhs = _methodBuilder->getIlValue(I.getOperand(1));
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
