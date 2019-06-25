#ifndef LLJB_MODULE_HPP
#define LLJB_MODULE_HPP

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"

#include <memory>


namespace llvm{
    class LLVMContext;
    class SMDiagnostic;
}

namespace lljb {

class Module {
public:
    Module(const char * filename, llvm::SMDiagnostic &SMDiags, llvm::LLVMContext &context);

    std::unique_ptr<llvm::Module>& getLLVMModule(){return _llvmModule;}

    int32_t numFunctions(){ return _llvmModule->getFunctionList().size();}
    //int32_t numGlobals(){}
    llvm::simple_ilist<llvm::Function>::iterator
        funcIterBegin(){return _llvmModule->getFunctionList().begin();}
    llvm::simple_ilist<llvm::Function>::iterator
        funcIterEnd(){return _llvmModule->getFunctionList().end();}
    llvm::Function * getMainFunction();


private:

    std::unique_ptr<llvm::Module> _llvmModule;
    bool _constructed;
    const char * _filename;

}; /* class Module */

} /* namespace lljb */


#endif /* LLJB_MODULE_HPP */