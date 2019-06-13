#include "lljb/IRVisitor.hpp"

#include <cassert>

namespace lljb {

void IRVisitor::visitInstruction(llvm::Instruction &I){
    assert("Unimplemented instruction!\n");
}

void IRVisitor::visitReturnInst(llvm::ReturnInst &I){
    llvm::outs() << "return inst: " << I << "\n";
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = _methodBuilder->getIlValue(value);

    if (!ilValue) ilValue = createConstIntIlValue(value);

    _methodBuilder->Return(ilValue);
}

void IRVisitor::visitAllocaInst(llvm::AllocaInst &I){
    llvm::outs() << "alloca inst: " << I << "\n";
    _methodBuilder->mapIRtoILValue(&I, nullptr);
}

void IRVisitor::visitLoadInst(llvm::LoadInst &I){
    llvm::outs() << "load inst: " << I << "\n";
    llvm::Value * source = I.getPointerOperand();
    TR::IlValue * ilValue = _methodBuilder->getIlValue(source);
    _methodBuilder->mapIRtoILValue(&I, ilValue);
}

void IRVisitor::visitStoreInst(llvm::StoreInst &I){
    llvm::outs() << "store inst: " << I << "\n";
    llvm::Value * dest = I.getPointerOperand();
    llvm::Value * value = I.getOperand(0);
    TR::IlValue * ilValue = _methodBuilder->getIlValue(value);

    if (!ilValue) ilValue = createConstIntIlValue(value);

    _methodBuilder->mapIRtoILValue(dest, ilValue);
}

void IRVisitor::visitBinaryOperator(llvm::BinaryOperator &I){
    llvm::outs() << "binaryOp inst: " << I << "\n";
    TR::IlValue * lhs = _methodBuilder->getIlValue(I.getOperand(0));
    TR::IlValue * rhs = _methodBuilder->getIlValue(I.getOperand(1));
    TR::IlValue * result = nullptr;
    switch (I.getOpcode()){
        case llvm::BinaryOperator::Add:
            result = _methodBuilder->Add(lhs,rhs);
            break;
        case llvm::BinaryOperator::Sub:
            result = _methodBuilder->Sub(lhs, rhs);
            break;
        case llvm::BinaryOperator::Mul:
            result = _methodBuilder->Mul(lhs, rhs);
            break;
        case llvm::BinaryOperator::SDiv:
            result = _methodBuilder->Div(lhs, rhs);
            break;
        default:
            assert("Unknown binary operand");
            break;
    }
    _methodBuilder->mapIRtoILValue(&I, result);
}

TR::IlValue * IRVisitor::createConstIntIlValue(llvm::Value * value){
    TR::IlValue * ilValue = nullptr;
    llvm::ConstantInt * constInt = llvm::dyn_cast<llvm::ConstantInt>(value);
    int64_t signExtendedValue = constInt->getSExtValue();
    if (constInt->getBitWidth() <= 8) ilValue = _methodBuilder->ConstInt8(signExtendedValue);
    else if (constInt->getBitWidth() <= 16) ilValue = _methodBuilder->ConstInt16(signExtendedValue);
    else if (constInt->getBitWidth() <= 32) ilValue = _methodBuilder->ConstInt32(signExtendedValue);
    else if (constInt->getBitWidth() <= 64) ilValue = _methodBuilder->ConstInt64(signExtendedValue);

    return ilValue;
}

} /* namespace lljb */
