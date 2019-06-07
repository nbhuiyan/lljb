#ifndef LLJB_MODULE_HPP
#define LLJB_MODULE_HPP

#include "llvm/IR/Module.h"

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


private:

    std::unique_ptr<llvm::Module> _llvmModule;
    bool _constructed;
    const char * _filename;

}; /* class Module */

} /* namespace lljb */


#endif /* LLJB_MODULE_HPP */