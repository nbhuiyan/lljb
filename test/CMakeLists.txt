add_executable(lljb_run
    lljb_run.cpp
)

target_include_directories(lljb_run
    PRIVATE
        ${CMAKE_SOURCE_DIR}/third_party/llvm/include/
        ${CMAKE_BINARY_DIR}/third_party/llvm/include/
        ${CMAKE_SOURCE_DIR}/third_party/omr/jitbuilder/
        ${CMAKE_SOURCE_DIR}/third_party/omr/compiler/
        ${CMAKE_SOURCE_DIR}/third_party/omr/
)

target_link_libraries(lljb_run
    lljb
)

add_subdirectory(cpp)