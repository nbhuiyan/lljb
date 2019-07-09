#include "lljb/Compiler.hpp"
#include "lljb/MethodBuilder.hpp"

#include "llvm/IR/Function.h"

namespace lljb{

Compiler::Compiler(Module * module)
    : _typeDictionary(),
      _module(module){}

void Compiler::compile(){
    for (auto func = _module->funcIterBegin(); func != _module->funcIterEnd() ; func++){
        if (!(*func).isDeclaration())
            mapCompiledFunction(&*func,(compileMethod(*func)));
    }
}

JittedFunction Compiler::getJittedCodeEntry(){
    llvm::Function * entryFunction = _module->getMainFunction();
    assert(entryFunction && "entryfunction not found");
    return getCompiledFunctionEntry(entryFunction);
}

JittedFunction Compiler::compileMethod(llvm::Function &func){
    MethodBuilder methodBuilder(&_typeDictionary, func, this);
    void * result = 0;
    methodBuilder.Compile(&result);
    return (JittedFunction) result;
}

void Compiler::mapCompiledFunction(llvm::Function * llvmFunc, JittedFunction entry){
    _compiledFunctionMap[llvmFunc] = entry;
}

JittedFunction Compiler::getCompiledFunctionEntry(llvm::Function * func){
    return _compiledFunctionMap[func];
}


} /** namespace lljb */
