#include "lljb/MethodBuilder.hpp"
#include "lljb/IRVisitor.hpp"

namespace lljb {

MethodBuilder::MethodBuilder(TR::TypeDictionary *td, LLJBTypes * lltd, llvm::Function &func)
    : TR::MethodBuilder(td),
      _function(func) {
    DefineLine("<unknown>");
    DefineFile("unknown");
    DefineName(func.getName().str().c_str());
    DefineReturnType(lltd->i32); //todo create a type mapping between llvm and lljb
}

bool MethodBuilder::buildIL(){
    IRVisitor visitor(this);
    visitor.visit(_function);
    return true;
}

} /* namespace lljb */