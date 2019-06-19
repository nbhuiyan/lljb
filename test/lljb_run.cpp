#include "lljb/Module.hpp"
#include "lljb/Compiler.hpp"
#include "lljb/IRVisitor.hpp"
#include "JitBuilder.hpp"

#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"

#include <iostream>
#include <cassert>

int main(int argc, char * argv[]){

    bool jitInitialized = initializeJit();
    assert(jitInitialized && "Jit Failed to initialize");

    const char * filename = argv[1]; //todo: better input handling.. this is temporary

    llvm::LLVMContext context;
    llvm::SMDiagnostic SMDiags;
    lljb::Module module(filename, SMDiags, context);

    lljb::Compiler compiler(&module);
    compiler.compile();

    lljb::JittedFunction jc = compiler.getFunction(0);
    auto result = jc();
    std::cout << "return value: " << result << std::endl;

    shutdownJit();

    return 0;
}