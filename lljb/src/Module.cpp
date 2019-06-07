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

    //lljb::IRVisitor visitor;
    //visitor.visit(*_llvmModule);
    //for (auto func = _llvmModule->getFunctionList().begin(); func != _llvmModule->getFunctionList().end() ; func++){
    //    visitor.visit(*func);
    //}
}

} // namespace lljb