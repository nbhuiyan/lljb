#include "ilgen/TypeDictionary.hpp"

#include "lljb/MethodBuilder.hpp"
#include "lljb/IRVisitor.hpp"

namespace lljb {


MethodBuilder::MethodBuilder(TR::TypeDictionary *td, llvm::Function &func)
    : TR::MethodBuilder(td),
      _function(func){
    DefineFile(func.getParent()->getSourceFileName().data());
    DefineLine("n/a"); // line numer is only available if the bitcode file was
                       // generated with debug info, hence not dealing with that
    DefineName(func.getName().data());
    DefineReturnType(getLLJBType(func.getReturnType()));
    setUseBytecodeBuilders();
}

bool MethodBuilder::buildIL(){
    assignBuildersToBasicBlocks();

    for (auto bbIterator = _function.begin(); bbIterator != _function.end(); bbIterator++){
        TR::BytecodeBuilder * builder = _BBToBuilderMap[&(*bbIterator)];
        IRVisitor visitor(this, builder);
        visitor.visit(*bbIterator);
    }

    return true;
}

TR::IlType * MethodBuilder::getLLJBType(llvm::Type * type){
    if (type->isIntegerTy()){
        if (type->isIntegerTy(8)) return typeDictionary()->Int8;
        else if (type->isIntegerTy(16)) return typeDictionary()->Int16;
        else if (type->isIntegerTy(32)) return typeDictionary()->Int32;
        else if (type->isIntegerTy(64)) return typeDictionary()->Int64;
        else assert(0 && "Unsupported integer type");
    }
    else assert (0 && "Unsupported type");
}

void MethodBuilder::assignBuildersToBasicBlocks(){
    for (auto bbIterator = _function.begin(); bbIterator != _function.end(); bbIterator++){
        _BBToBuilderMap[&(*bbIterator)] = OrphanBytecodeBuilder();
    }
    AppendBytecodeBuilder(_BBToBuilderMap[&(*_function.begin())]);
}

TR::IlValue * MethodBuilder::getIlValue(llvm::Value * value){
    return _valueMap[value];
}

void MethodBuilder::mapIRtoIlValue(llvm::Value * irValue, TR::IlValue * ilValue){
    _valueMap[irValue] = ilValue;
}

TR::BytecodeBuilder * MethodBuilder::getByteCodeBuilder(llvm::Value * value){
    return _BBToBuilderMap[value];
}

} /* namespace lljb */