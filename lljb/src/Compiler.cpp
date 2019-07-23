#include "lljb/Compiler.hpp"
#include "lljb/MethodBuilder.hpp"

#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdio>
#include <ctime>

#if defined(OSX) || defined(LINUX)
#include <unistd.h>
#endif

#include <string>
#include <vector>

namespace lljb{

Compiler::Compiler(Module * module)
    : _typeDictionary(),
      _module(module){
          defineTypes();
}

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
        else if (func->getName().equals("clock_gettime")) entry = (void *) &clock_gettime; // c/cpp
#if defined(OSX) || defined(LINUX)
        else if (func->getName().contains("usleep")) entry = (void *) &usleep; // c/cpp
#endif
        else assert( 0 && "function not found");
    }
    return entry;
}

char * Compiler::stringifyObjectMemberIndex(unsigned memberIndex){
    // All structs will share the same name for their members. First check if a member for an
    // index has already been generated before creating a new one.
    char * memberString = _objectMemberMap[memberIndex];
    if (memberString) return memberString;

    std::string indexString = std::to_string(memberIndex);
    memberString = new char[indexString.length()+2];
    sprintf(memberString, "m%d",memberIndex);
    _objectMemberMap[memberIndex] = memberString;
    return memberString;
}

void Compiler::defineTypes(){
    std::vector<llvm::StructType*> structTypes = _module->getLLVMModule()->getIdentifiedStructTypes();
    for (auto structType : structTypes){
        llvm::StringRef structName = structType->getName();
        _typeDictionary.DefineStruct(structName.data());
        unsigned elIndex = 0;
        for (auto elIter = structType->element_begin(); elIter != structType->element_end(); ++elIter){
            _typeDictionary.DefineField(
                    structName.data(),
                    stringifyObjectMemberIndex(elIndex),
                    MethodBuilder::getIlType(&_typeDictionary,structType->getElementType(elIndex)));
            elIndex++;
        }
        _typeDictionary.CloseStruct(structName.data());
    }
}

char * Compiler::getObjectMemberNameFromIndex(unsigned index){
    return _objectMemberMap[index];
}

Compiler::~Compiler(){
    _objectMemberMap.clear();
}


} /** namespace lljb */
