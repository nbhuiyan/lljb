#ifndef LLJB_METHODBUILDER_HPP
#define LLJB_METHODBUILDER_HPP

#include "ilgen/MethodBuilder.hpp"
#include "lljb/LLJBTypes.hpp"

#include "llvm/IR/Function.h"


namespace lljb {

class MethodBuilder : public TR::MethodBuilder {

public:
    MethodBuilder(TR::TypeDictionary *td, LLJBTypes * lltd, llvm::Function &func);

    virtual bool buildIL() override;

private:
    llvm::Function& _function;

}; /* class MethodBuilder */

} /* namespace lljb */



#endif /* LLJB_METHODBUILDER_HPP */