#include "lljb/Module.hpp"
#include "lljb/MethodBuilder.hpp"
#include "lljb/Compiler.hpp"
#include "lljb/IRVisitor.hpp"
#include "JitBuilder.hpp"

#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"


#include <iostream>
#include <cstdio>
#include <cassert>

#include <memory>

int main(){

    bool jitInitialized = initializeJit();
    if (!jitInitialized) assert("Jit Failed to initialize");


    const char * filename = "/Users/nazim/Desktop/testfield/llvm-ir/test.ll"; //temporary.. to be read from argv[1] later
    llvm::LLVMContext context;
    llvm::SMDiagnostic SMDiags;
    lljb::Module module(filename, SMDiags, context);

    //lljb::JittedFunction jc = compiler.generateCode(&module);
    lljb::Compiler compiler(&module);
    compiler.compile();

    lljb::JittedFunction jc = compiler.getFunction(0);
    auto result = jc();
    std::cout << "return value: " << result << std::endl;

    shutdownJit();

    return 0;
}