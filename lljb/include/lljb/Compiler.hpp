#ifndef LLJB_COMPILER_HPP
#define LLJB_COMPILER_HPP

#include "lljb/LLJBTypes.hpp"
#include "lljb/Module.hpp"

#include <cstdint>
#include <memory>
#include <vector>

namespace lljb {

typedef int32_t (*JittedFunction)();

class Compiler{
public:
    Compiler(Module * module);

    void compile();

    JittedFunction getFunction(int32_t index){ return _compiledFunctions[index]; }

private:
    JittedFunction compileMethod(llvm::Function &func);

    TR::TypeDictionary _typeDictionary;
    LLJBTypes _lljbtypes;
    std::vector<JittedFunction> _compiledFunctions;
    Module * _module;
};

} /** namespace lljb */

#endif /* LLJB_COMPILER_HPP */
