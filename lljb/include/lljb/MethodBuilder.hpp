#ifndef LLJB_METHODBUILDER_HPP
#define LLJB_METHODBUILDER_HPP

#include "ilgen/MethodBuilder.hpp"

#include "llvm/IR/Function.h"


namespace lljb {

class MethodBuilder : public TR::MethodBuilder {

public:
    MethodBuilder(TR::TypeDictionary *td, llvm::Function &func);

    TR::IlType * getLLJBType(llvm::Type * type);

    virtual bool buildIL() override;

private:
    llvm::Function& _function;

}; /* class MethodBuilder */

} /* namespace lljb */



#endif /* LLJB_METHODBUILDER_HPP */