#include "lljb/IRVisitor.hpp"

#include <cassert>

namespace lljb {

void IRVisitor::visitInstruction(llvm::Instruction &I){
    assert("Unimplemented instruction!\n");
}

void IRVisitor::visitReturnInst(llvm::ReturnInst &I){
    llvm::Value * value = I.getOperand(0);
    llvm::Type * llvmValueType = value->getType();
    TR::IlValue * ilValue = nullptr;

    if (llvmValueType->isIntegerTy()){
        llvm::ConstantInt * constInt = llvm::dyn_cast<llvm::ConstantInt>(value);
        int64_t signExtendedValue = constInt->getSExtValue();
        if (constInt->getBitWidth() <= 8) ilValue = _methodBuilder->ConstInt8(signExtendedValue);
        else if (constInt->getBitWidth() <= 16) ilValue = _methodBuilder->ConstInt16(signExtendedValue);
        else if (constInt->getBitWidth() <= 32) ilValue = _methodBuilder->ConstInt32(signExtendedValue);
        else if (constInt->getBitWidth() <= 64) ilValue = _methodBuilder->ConstInt64(signExtendedValue);
        else assert("Unsupported operand size");
    }
    else {
        assert("Only constant interger return type is supported\n");
    }

    _methodBuilder->Return(ilValue);
}

} /* namespace lljb */
