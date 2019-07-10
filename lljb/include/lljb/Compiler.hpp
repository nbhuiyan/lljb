#ifndef LLJB_COMPILER_HPP
#define LLJB_COMPILER_HPP

#include "lljb/Module.hpp"
#include "ilgen/TypeDictionary.hpp"

#include "llvm/ADT/DenseMap.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace lljb {

typedef int32_t (*JittedFunction)();

class Compiler{
public:
    Compiler(Module * module);

    void compile();

    JittedFunction getJittedCodeEntry();

    void mapCompiledFunction(llvm::Function * llvmFunc, void * entry);

    void * getFunctionAddress(llvm::Function * func);

private:
    void * compileMethod(llvm::Function &func);

    TR::TypeDictionary _typeDictionary;
    llvm::DenseMap<llvm::Function *, void *> _compiledFunctionMap;
    Module * _module;
};

} /** namespace lljb */

#endif /* LLJB_COMPILER_HPP */
