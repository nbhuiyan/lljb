#ifndef LLJB_METHODBUILDER_HPP
#define LLJB_METHODBUILDER_HPP

#include "ilgen/MethodBuilder.hpp"
#include "ilgen/BytecodeBuilder.hpp"
#include "ilgen/VirtualMachineState.hpp"

#include "llvm/IR/Function.h"
#include "llvm/ADT/DenseMap.h"

namespace lljb {

class State : public TR::VirtualMachineState {
    public:

    virtual void Commit(TR::IlBuilder *b) override final {}

    virtual void Reload(TR::IlBuilder *b) override final {}

    virtual VirtualMachineState *MakeCopy() override final {
    return new State(*this);
    }

    virtual void MergeInto(TR::VirtualMachineState *other,
                           TR::IlBuilder *b) override final {}
};

class MethodBuilder : public TR::MethodBuilder {

public:
    MethodBuilder(TR::TypeDictionary *td, llvm::Function &func);
    TR::IlType * getLLJBType(llvm::Type * type);
    virtual bool buildIL() override;
    TR::IlValue * getIlValue(llvm::Value * value);
    void mapIRtoIlValue(llvm::Value * irValue, TR::IlValue * ilValue);
    TR::BytecodeBuilder * getByteCodeBuilder(llvm::Value * value);

private:
    void assignBuildersToBasicBlocks();

    llvm::Function& _function;
    llvm::DenseMap<llvm::Value *, TR::BytecodeBuilder * > _BBToBuilderMap;
    llvm::DenseMap<llvm::Value *, TR::IlValue *> _valueMap;

}; /* class MethodBuilder */

} /* namespace lljb */



#endif /* LLJB_METHODBUILDER_HPP */
