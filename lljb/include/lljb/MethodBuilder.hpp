#ifndef LLJB_METHODBUILDER_HPP
#define LLJB_METHODBUILDER_HPP

#include "ilgen/MethodBuilder.hpp"
#include "ilgen/BytecodeBuilder.hpp"
#include "ilgen/VirtualMachineState.hpp"

#include "llvm/IR/Function.h"
#include "llvm/ADT/DenseMap.h"

namespace lljb {

class Compiler;

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
    MethodBuilder(TR::TypeDictionary *td, llvm::Function &func, Compiler * compiler);
    ~MethodBuilder();

    static TR::IlType * getIlType(TR::TypeDictionary * td,llvm::Type * type);

    virtual bool buildIL() override;
    TR::IlValue * getIlValue(llvm::Value * value);
    char * getParamNameFromIndex(unsigned index);
    void mapIRtoIlValue(llvm::Value * irValue, TR::IlValue * ilValue);
    TR::BytecodeBuilder * getByteCodeBuilder(llvm::Value * value);
    void defineFunction(llvm::Function * func);
    char * getMemberNameFromIndex(unsigned index);

private:
    void assignBuildersToBasicBlocks();
    void defineParameters();

    /**
     * @brief Generate a null-terminated parameter name using the index of the
     * parameter. For example, if paramIndex is 1, then the generated parameter
     * name would be "p1\0"
     *
     * @param paramIndex
     * @return const char*
     */
    char * stringifyParamIndex(unsigned paramIndex);

    llvm::Function& _function;
    llvm::DenseMap<llvm::Value *, TR::BytecodeBuilder * > _BBToBuilderMap;
    llvm::DenseMap<llvm::Value *, TR::IlValue *> _valueMap;
    llvm::DenseMap<unsigned, char *> _parameterMap;
    llvm::DenseMap<llvm::Function *, void *> _definedFunctions;
    Compiler * _compiler;

}; /* class MethodBuilder */

} /* namespace lljb */



#endif /* LLJB_METHODBUILDER_HPP */
