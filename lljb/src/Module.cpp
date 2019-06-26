#include "lljb/Module.hpp"

#include "llvm/Support/SourceMgr.h"
#include "llvm/IRReader/IRReader.h"

#include "llvm/IR/Function.h" //temp
#include "llvm/IR/BasicBlock.h" //temp

#include "lljb/IRVisitor.hpp" //temp


namespace lljb{

Module::Module(const char * filename, llvm::SMDiagnostic &SMDiags, llvm::LLVMContext &context)
            : _llvmModule(nullptr),
              _constructed(false),
              _filename(filename) {
    _llvmModule = (llvm::parseIRFile(_filename, SMDiags, context));
    if (!_llvmModule){
        SMDiags.print(_filename, llvm::errs());
    }
    else {
        _constructed = true;
    }
    llvm::outs() << "Module loaded:" << _llvmModule->getModuleIdentifier() << "\n";

}

llvm::Function * Module::getMainFunction(){
    if (numFunctions() == 1)
        return &(*(funcIterBegin()));
    return _llvmModule->getFunction("main");
}

} // namespace lljb