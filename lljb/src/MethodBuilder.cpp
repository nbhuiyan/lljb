#include "ilgen/TypeDictionary.hpp"

#include "lljb/MethodBuilder.hpp"
#include "lljb/IRVisitor.hpp"
#include "lljb/Compiler.hpp"

#include <string>
#include <cstdio>
#include <vector>

namespace lljb {


MethodBuilder::MethodBuilder(TR::TypeDictionary *td, llvm::Function &func, Compiler * compiler)
    : TR::MethodBuilder(td),
      _function(func),
      _compiler(compiler){
    DefineFile(func.getParent()->getSourceFileName().data());
    DefineLine("n/a"); // line numer is only available if the bitcode file was
                       // generated with debug info, hence not dealing with that
    DefineName(func.getName().data());
    DefineReturnType(getIlType(func.getReturnType()));
    setUseBytecodeBuilders();
    defineParameters();
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

TR::IlType * MethodBuilder::getIlType(llvm::Type * type){
    TR::IlType * ilType = nullptr;
    switch (type->getTypeID()){
        case llvm::Type::TypeID::IntegerTyID: // arbitrary bitwidth integers
            if (type->isIntegerTy(8)) ilType = typeDictionary()->Int8;
            else if (type->isIntegerTy(16)) ilType = typeDictionary()->Int16;
            else if (type->isIntegerTy(32)) ilType = typeDictionary()->Int32;
            else if (type->isIntegerTy(64)) ilType = typeDictionary()->Int64;
            else assert(0 && "Unsupported integer type");
            break;
        case llvm::Type::TypeID::FloatTyID: // 32-bit floating point type
            ilType = typeDictionary()->Float;
            break;
        case llvm::Type::TypeID::DoubleTyID: // 64-bit floating point type
            ilType = typeDictionary()->Double;
            break;
        case llvm::Type::TypeID::PointerTyID: // Pointers
            ilType = typeDictionary()->PointerTo(getIlType(type->getPointerElementType()));
            break;
        case llvm::Type::TypeID::LabelTyID: // Label type
        case llvm::Type::TypeID::HalfTyID: // 16-bit floating point type
        case llvm::Type::TypeID::X86_FP80TyID: // 80-bit floating point type (X87)
        case llvm::Type::TypeID::FP128TyID: // 128-bit floating point type (112-bit mantissa)
        case llvm::Type::TypeID::PPC_FP128TyID: // 128-bit floating point type (2 64-bits) -- PPC
        case llvm::Type::TypeID::X86_MMXTyID: // 64-bit MMX vectors -- X86
        case llvm::Type::TypeID::TokenTyID: // Tokens
        case llvm::Type::TypeID::FunctionTyID: // Functions
        case llvm::Type::TypeID::StructTyID: // Structures
        case llvm::Type::TypeID::ArrayTyID: // Arrays
        case llvm::Type::TypeID::VectorTyID: // SIMD "packed" format, or other vector types
        case llvm::Type::TypeID::MetadataTyID: // Metadata type
        case llvm::Type::TypeID::VoidTyID: // type with no size
        default:
            assert(0 && "Unsupported llvm type!");
            break;
    }
    return ilType;
}

void MethodBuilder::assignBuildersToBasicBlocks(){
    int32_t index = 0;

    for (llvm::BasicBlock &BB : _function){
        TR::BytecodeBuilder * currentBuilder = OrphanBytecodeBuilder(index);
        _BBToBuilderMap[&BB] = currentBuilder;
        index ++;
    }
}

void MethodBuilder::defineParameters(){
    for (auto arg = _function.arg_begin(); arg != _function.arg_end(); ++arg){
        DefineParameter(stringifyParamIndex(arg->getArgNo()), getIlType(arg->getType()));
    }
}

char * MethodBuilder::stringifyParamIndex(unsigned paramIndex){
    std::string indexString = std::to_string(paramIndex);
    char * paramString = new char[indexString.length()+2];
    sprintf(paramString, "p%d",paramIndex);
    _parameterMap[paramIndex] = paramString;
    return paramString;
}

char * MethodBuilder::getParamNameFromIndex(unsigned index){
    return _parameterMap[index];
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

void MethodBuilder::defineFunction(llvm::Function * func){
    if (_definedFunctions[func]) return;
    const char * name = func->getName().data();
    const char * fileName = func->getParent()->getSourceFileName().data();
    const char * lineNumer = "n/a";
    void * entry = _compiler->getFunctionAddress(func);
    TR::IlType * returnType = getIlType(func->getReturnType());
    std::size_t  numArgs = func->arg_size();
    if (!numArgs){
        DefineFunction(name,
                        fileName,
                        lineNumer,
                        entry,
                        returnType,
                        0);
    }
    else {
        std::vector<TR::IlType *> argTypes;
        for (auto arg = func->arg_begin(); arg != func->arg_end(); ++arg){
            argTypes.push_back(getIlType(arg->getType()));
        }

        DefineFunction(name,
                        fileName,
                        lineNumer,
                        entry,
                        returnType,
                        numArgs,
                        argTypes.data());

    }
    _definedFunctions[func] = entry;
}

MethodBuilder::~MethodBuilder(){
    _parameterMap.clear();
}

} /* namespace lljb */
