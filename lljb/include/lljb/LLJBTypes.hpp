#ifndef LLJB_TYPEDICTIONARY_HPP
#define LLJB_TYPEDICTIONARY_HPP

#include "ilgen/TypeDictionary.hpp"

namespace lljb{

class LLJBTypes {
public:
    LLJBTypes(TR::TypeDictionary &td);

    TR::IlType *i32;

}; /* class LLJBTypes */

} /* namespace lljb */

#endif /* LLJB_TYPEDICTIONARY_HPP */