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
    State * state = new State();
    setVMState(state);
    assignBuildersToBasicBlocks();

    TR::BytecodeBuilder * firstBuilder = _BBToBuilderMap[&(_function.getEntryBlock())];
    assert (firstBuilder && "first builder not found!");
    AppendBytecodeBuilder(firstBuilder);

    for (llvm::BasicBlock &BB : _function){
        TR::BytecodeBuilder * builder = _BBToBuilderMap[&BB];
        builder->setVMState(state); // why does every bytecodebuilder need this set?
        IRVisitor visitor(this, builder,typeDictionary());
        visitor.visit(BB);
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
    int32_t index = 0;

    for (llvm::BasicBlock &BB : _function){
        TR::BytecodeBuilder * currentBuilder = OrphanBytecodeBuilder(index);
        _BBToBuilderMap[&BB] = currentBuilder;
        index ++;
    }
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