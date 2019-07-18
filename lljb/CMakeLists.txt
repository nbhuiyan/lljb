add_library(lljb SHARED
    src/Compiler.cpp
    src/MethodBuilder.cpp
    src/Module.cpp
    src/IRVisitor.cpp
)

target_include_directories(lljb
    PUBLIC
        include/
        ${CMAKE_SOURCE_DIR}/third_party/llvm/include/
        ${CMAKE_BINARY_DIR}/third_party/llvm/include/
        ${CMAKE_SOURCE_DIR}/third_party/omr/jitbuilder/
        ${CMAKE_SOURCE_DIR}/third_party/omr/compiler/
        ${CMAKE_SOURCE_DIR}/third_party/omr/
)

target_link_libraries(lljb
    jitbuilder
    LLVMIRReader
    LLVMCore
    LLVMSupport
)
