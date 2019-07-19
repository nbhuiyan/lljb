#include "lljb/Module.hpp"

#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/SourceMgr.h"


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
