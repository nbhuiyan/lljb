#include "ilgen/TypeDictionary.hpp"

#include "lljb/MethodBuilder.hpp"
#include "lljb/IRVisitor.hpp"

namespace lljb {


MethodBuilder::MethodBuilder(TR::TypeDictionary *td, llvm::Function &func)
    : TR::MethodBuilder(td),
      _function(func),
      _valueMap(){
    DefineLine("l");
    DefineFile("f");
    DefineName(func.getName().data());
    DefineReturnType(getLLJBType(func.getReturnType())); //todo create a type mapping between llvm and lljb
    AllLocalsHaveBeenDefined();
}

bool MethodBuilder::buildIL(){
    IRVisitor visitor(this);
    visitor.visit(_function);
    return true;
}

TR::IlType * MethodBuilder::getLLJBType(llvm::Type * type){
    if (type->isIntegerTy()){
        if (type->isIntegerTy(8)) return typeDictionary()->Int8;
        else if (type->isIntegerTy(16)) return typeDictionary()->Int16;
        else if (type->isIntegerTy(32)) return typeDictionary()->Int32;
        else if (type->isIntegerTy(64)) return typeDictionary()->Int64;
        else assert("Unsupported integer type");
    }
    else assert ("Unsupported type");
}

TR::IlValue * MethodBuilder::getIlValue(llvm::Value * value){
    return _valueMap[value];
}

void MethodBuilder::mapIRtoILValue(llvm::Value * llvmValue, TR::IlValue * ilValue){
    _valueMap[llvmValue] = ilValue;
}

} /* namespace lljb */