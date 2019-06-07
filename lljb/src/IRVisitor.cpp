#include "lljb/IRVisitor.hpp"

#include <cassert>

namespace lljb {

void IRVisitor::visitInstruction(llvm::Instruction &I){
    assert("Unimplemented instruction!\n");
}

void IRVisitor::visitReturnInst(llvm::ReturnInst &I){
    llvm::outs() << "return instruction\n";
    llvm::Value * value = I.getOperand(0);
    if (llvm::ConstantInt* CI = llvm::dyn_cast<llvm::ConstantInt>(value)) {
        if (CI->getBitWidth() <= 32){
            int32_t constIntValue = CI->getSExtValue();
            TR::IlValue * ilValue = _methodBuilder->ConstInt32(constIntValue);
            _methodBuilder->Return(ilValue);
        }
    }
}

} /* namespace lljb */
