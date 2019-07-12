#ifndef LLJB_IRVISITOR_HPP
#define LLJB_IRVISITOR_HPP

#include "lljb/MethodBuilder.hpp"

#include "llvm/IR/InstVisitor.h"

namespace lljb {

struct IRVisitor : public llvm::InstVisitor<IRVisitor> {
    IRVisitor(MethodBuilder* methodBuilder, TR::BytecodeBuilder* builder, TR::TypeDictionary * td)
        :_methodBuilder(methodBuilder),
         _builder(builder),
         _td(td)
    {}

    /************************************************************************
     *
     *  LLVM IR Instruction visitors
     *
     ************************************************************************/

    /************************************************************************
     * Implemented visitors
     ************************************************************************/
    void visitReturnInst(llvm::ReturnInst &I);
    void visitAllocaInst(llvm::AllocaInst &I);
    void visitLoadInst(llvm::LoadInst     &I);
    void visitStoreInst(llvm::StoreInst   &I);
    void visitBinaryOperator(llvm::BinaryOperator &I);

    /**
     * visitCmpInst handles the following instructions:
     * llvm::ICmpInst
     * llvm::FCmpInst
     */
    void visitCmpInst(llvm::CmpInst &I); // handles both llvm::ICmpInst and llvm::FCmpInst

    void visitBranchInst(llvm::BranchInst &I);
    void visitCallInst(llvm::CallInst &I);

    /**
     * visitCastInst handles the following instructions:
     * llvm::AddrSpaceCastInst
     * llvm::BitCastInst
     * llvm::FPExtInst
     * llvm::FPToSIInst
     * llvm::FPToUIInst
     * llvm::FPTruncInst
     * llvm::IntToPtrInst
     * llvm::PtrToIntInst
     * llvm::SExtInst
     * llvm::SIToFPInst
     * llvm::TruncInst
     * llvm::UIToFPInst
     * llvm::ZExtInst
     */
    void visitCastInst(llvm::CastInst &I);

    void visitGetElementPtrInst(llvm::GetElementPtrInst &I);


    /************************************************************************
     * Unimplemented visitors
     ************************************************************************/

    /* Handles cases of visiting an unimplemented instruction */
    void visitInstruction(llvm::Instruction &I);

    //void visitUnaryOperator(llvm::UnaryOperator &I);
    //void visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &I);
    //void visitAtomicRMWInst(llvm::AtomicRMWInst &I);
    //void visitFenceInst(llvm::FenceInst   &I);
    //void visitPHINode(llvm::PHINode       &I);
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
    //void visitInvokeInst(llvm::InvokeInst &I);
    //void visitSwitchInst(llvm::SwitchInst &I);
    //void visitIndirectBrInst(llvm::IndirectBrInst &I);
    //void visitResumeInst(llvm::ResumeInst &I);
    //void visitUnreachableInst(llvm::UnreachableInst &I);
    //void visitCleanupReturnInst(llvm::CleanupReturnInst &I);
    //void visitCatchReturnInst(llvm::CatchReturnInst &I);
    //void visitCatchSwitchInst(llvm::CatchSwitchInst &I);
    //void visitTerminator(llvm::Instruction &I);
    //void visitUnaryInstruction(llvm::UnaryInstruction &I);
    //void visitCallBase(llvm::CallBase &I);
    //void visitCallSite(llvm::CallSite CS);



private:
    /**
     * Helpers
     */

    TR::IlValue * createConstIntIlValue(llvm::Value * value);
    TR::IlValue * createConstFPIlValue(llvm::Value * value);
    TR::IlValue * createConstExprIlValue(llvm::Value * value);
    TR::IlValue * createConstantDataArrayVal(llvm::Value * value);
    TR::IlValue * loadParameter(llvm::Value * value);
    TR::IlValue * loadGlobal(llvm::Value * value);
    TR::IlValue * getIlValue(llvm::Value * value);

    /**
     * Private fields
     */
    MethodBuilder * _methodBuilder;
    TR::BytecodeBuilder * _builder;
    TR::TypeDictionary * _td;


}; // struct IRVisitor

}

#endif
