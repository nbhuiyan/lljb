#include "lljb/Compiler.hpp"
#include "lljb/MethodBuilder.hpp"

#include "llvm/IR/Function.h"

#include <cstdio>

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
    assert(entryFunction && "entry function not found");
    return (JittedFunction) getFunctionAddress(entryFunction);
}

void * Compiler::compileMethod(llvm::Function &func){
    MethodBuilder methodBuilder(&_typeDictionary, func, this);
    void * result = 0;
    methodBuilder.Compile(&result);
    return result;
}

void Compiler::mapCompiledFunction(llvm::Function * llvmFunc, void * entry){
    _compiledFunctionMap[llvmFunc] = entry;
}

void * Compiler::getFunctionAddress(llvm::Function * func){
    void * entry = _compiledFunctionMap[func];
    if (!entry){ // temporary hack to call stdlib functions
        if (func->getName().equals("printf")) entry = (void *) &printf; // c/cpp
        else if (func->getName().equals("putc")) entry = (void *) &putc; // c/cpp
    }
    return entry;
}


} /** namespace lljb */
