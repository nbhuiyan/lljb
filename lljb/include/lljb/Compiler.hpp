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

    ~Compiler();

    void compile();

    JittedFunction getJittedCodeEntry();

    void mapCompiledFunction(llvm::Function * llvmFunc, void * entry);

    void * getFunctionAddress(llvm::Function * func);

    char * getObjectMemberNameFromIndex(unsigned index);

private:
    void defineTypes();
    void * compileMethod(llvm::Function &func);

    /**
     * @brief Generate a null-terminated parameter name using the index of an
     * aggregate type's member. This is needed because we refer to object fields
     * using their names. Example of generated name if member index is 1: "m1\0"
     *
     * @param memberIndex
     * @return char*
     */
    char * stringifyObjectMemberIndex(unsigned memberIndex);

    TR::TypeDictionary _typeDictionary;
    llvm::DenseMap<llvm::Function *, void *> _compiledFunctionMap;
    llvm::DenseMap<unsigned, char *> _objectMemberMap;
    Module * _module;
};

} /** namespace lljb */

#endif /* LLJB_COMPILER_HPP */
