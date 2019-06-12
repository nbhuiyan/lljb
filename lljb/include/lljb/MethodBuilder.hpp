#ifndef LLJB_METHODBUILDER_HPP
#define LLJB_METHODBUILDER_HPP

#include "ilgen/MethodBuilder.hpp"

#include "llvm/IR/Function.h"

#include <map>

namespace lljb {

class MethodBuilder : public TR::MethodBuilder {

public:
    MethodBuilder(TR::TypeDictionary *td, llvm::Function &func);
    TR::IlType * getLLJBType(llvm::Type * type);
    virtual bool buildIL() override;

    TR::IlValue * getIlValue(llvm::Value * value);
    void mapIRtoILValue(llvm::Value * llvmValue, TR::IlValue * ilValue);

private:
    llvm::Function& _function;
    std::map<llvm::Value *, TR::IlValue *> _valueMap;

}; /* class MethodBuilder */

} /* namespace lljb */



#endif /* LLJB_METHODBUILDER_HPP */
