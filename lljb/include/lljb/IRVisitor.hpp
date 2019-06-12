#ifndef LLJB_IRVISITOR_HPP
#define LLJB_IRVISITOR_HPP

#include "llvm/IR/InstVisitor.h"
#include "lljb/MethodBuilder.hpp"

namespace lljb {

struct IRVisitor : public llvm::InstVisitor<IRVisitor> {
    IRVisitor(MethodBuilder* methodBuilder)
        :_methodBuilder(methodBuilder)
    {}

    /************************************************************************
     *
     *  LLVM IR Instruction visitors
     *
     ************************************************************************/

    /**
     * Implemented visitors
     */
    void visitReturnInst(llvm::ReturnInst &I);
    void visitAllocaInst(llvm::AllocaInst &I);
    void visitLoadInst(llvm::LoadInst     &I);
    void visitStoreInst(llvm::StoreInst   &I);
    void visitBinaryOperator(llvm::BinaryOperator &I);

    /**
     * Unimplemented visitors
     */

    /* Handles cases of visiting an unimplemented instruction */
    void visitInstruction(llvm::Instruction &I);

    //void visitUnaryOperator(llvm::UnaryOperator &I);
    //void visitICmpInst(llvm::ICmpInst &I);
    //void visitFCmpInst(llvm::FCmpInst &I);
    //void visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &I);
    //void visitAtomicRMWInst(llvm::AtomicRMWInst &I);
    //void visitFenceInst(llvm::FenceInst   &I);
    //void visitGetElementPtrInst(llvm::GetElementPtrInst &I);
    //void visitPHINode(llvm::PHINode       &I);
    //void visitTruncInst(llvm::TruncInst &I);
    //void visitZExtInst(llvm::ZExtInst &I);
    //void visitSExtInst(llvm::SExtInst &I);
    //void visitFPTruncInst(llvm::FPTruncInst &I);
    //void visitFPExtInst(llvm::FPExtInst &I);
    //void visitFPToUIInst(llvm::FPToUIInst &I);
    //void visitFPToSIInst(llvm::FPToSIInst &I);
    //void visitUIToFPInst(llvm::UIToFPInst &I);
    //void visitSIToFPInst(llvm::SIToFPInst &I);
    //void visitPtrToIntInst(llvm::PtrToIntInst &I);
    //void visitIntToPtrInst(llvm::IntToPtrInst &I);
    //void visitBitCastInst(llvm::BitCastInst &I);
    //void visitAddrSpaceCastInst(llvm::AddrSpaceCastInst &I);
    //void visitSelectInst(llvm::SelectInst &I);
    //void visitVAArgInst(llvm::VAArgInst   &I);
    //void visitExtractElementInst(llvm::ExtractElementInst &I);
    //void visitInsertElementInst(llvm::InsertElementInst &I);
    //void visitShuffleVectorInst(llvm::ShuffleVectorInst &I);
    //void visitExtractValueInst(llvm::ExtractValueInst &I);
    //void visitInsertValueInst(llvm::InsertValueInst &I);
    //void visitLandingPadInst(llvm::LandingPadInst &I);
    //void visitFuncletPadInst(llvm::FuncletPadInst &I);
    //void visitCleanupPadInst(llvm::CleanupPadInst &I);
    //void visitCatchPadInst(llvm::CatchPadInst &I);
    //void visitDbgDeclareInst(llvm::DbgDeclareInst &I);
    //void visitDbgValueInst(llvm::DbgValueInst &I);
    //void visitDbgVariableIntrinsic(llvm::DbgVariableIntrinsic &I);
    //void visitDbgLabelInst(llvm::DbgLabelInst &I);
    //void visitDbgInfoIntrinsic(llvm::DbgInfoIntrinsic &I);
    //void visitMemSetInst(llvm::MemSetInst &I);
    //void visitMemCpyInst(llvm::MemCpyInst &I);
    //void visitMemMoveInst(llvm::MemMoveInst &I);
    //void visitMemTransferInst(llvm::MemTransferInst &I);
    //void visitMemIntrinsic(llvm::MemIntrinsic &I);
    //void visitVAStartInst(llvm::VAStartInst &I);
    //void visitVAEndInst(llvm::VAEndInst &I);
    //void visitVACopyInst(llvm::VACopyInst &I);
    //void visitIntrinsicInst(llvm::IntrinsicInst &I);
    //void visitCallInst(llvm::CallInst &I);
    //void visitInvokeInst(llvm::InvokeInst &I);
    //void visitBranchInst(llvm::BranchInst &I);
    //void visitSwitchInst(llvm::SwitchInst &I);
    //void visitIndirectBrInst(llvm::IndirectBrInst &I);
    //void visitResumeInst(llvm::ResumeInst &I);
    //void visitUnreachableInst(llvm::UnreachableInst &I);
    //void visitCleanupReturnInst(llvm::CleanupReturnInst &I);
    //void visitCatchReturnInst(llvm::CatchReturnInst &I);
    //void visitCatchSwitchInst(llvm::CatchSwitchInst &I);
    //void visitTerminator(llvm::Instruction &I);
    //void visitCastInst(llvm::CastInst &I);
    //void visitCmpInst(llvm::CmpInst &I);
    //void visitUnaryInstruction(llvm::UnaryInstruction &I);
    //void visitCallBase(llvm::CallBase &I);
    //void visitCallSite(llvm::CallSite CS);



private:
    MethodBuilder * _methodBuilder;

    /**
     * Helpers
     */

    TR::IlValue * createConstIntIlValue(llvm::Value * value);


}; // struct IRVisitor

}

#endif