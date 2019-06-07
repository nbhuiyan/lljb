cmake_minimum_required(VERSION 3.5)

list(APPEND CMAKE_MODULE_PATH
    cmake/modules
    ${CMAKE_CURRENT_LIST_DIR}/third_party/omr/cmake/modules/
)

project(lljb
    LANGUAGES C CXX
    VERSION 0.1
)

include(OmrPlatform)

omr_platform_global_setup()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

# OMR Configuration

set(OMR_JITBUILDER ON  CACHE BOOL "We use OMR Jitbuilder interface in lljb")
set(OMR_COMPILER   ON  CACHE BOOL "Enable building the OMR compiler library, needed for JitBuilder")
set(OMR_PORT ON CACHE BOOL "")
set(OMR_THREAD ON CACHE BOOL "")

set(OMR_FVTEST OFF CACHE BOOL "")
set(OMR_GC OFF CACHE BOOL "")
set(OMR_TEST_COMPILER OFF CACHE BOOL "")
set(OMR_OMRSIG OFF CACHE BOOL "")
set(OMR_GC_SEGREGATED_HEAP OFF CACHE BOOL "")
set(OMR_GC_MODRON_SCAVENGER OFF CACHE BOOL "")
set(OMR_GC_MODRON_CONCURRENT_MARK OFF CACHE BOOL "")
set(OMR_THR_CUSTOM_SPIN_OPTIONS OFF CACHE BOOL "")
set(OMR_NOTIFY_POLICY_CONTROL OFF CACHE BOOL "")
set(OMR_THR_SPIN_WAKE_CONTROL OFF CACHE BOOL "")
set(OMR_THR_SPIN_CODE_REFACTOR OFF CACHE BOOL "")
set(OMR_THR_FORK_SUPPORT OFF CACHE BOOL "")
set(OMR_WARNINGS_AS_ERRORS OFF CACHE BOOL "")
set(OMR_DDR OFF CACHE BOOL "")


# LLVM Configuration

# set(LLVM_BUILD_TOOLS OFF CACHE BOOL "")
# set(LLVM_BUILD_UTILS OFF CACHE BOOL "")

set(LLVM_SRC_ROOT ${CMAKE_CURRENT_LIST_DIR}/third_party/llvm/)
set(LLVM_OBJ_ROOT ${CMAKE_CURRENT_LIST_DIR}/build/third_party/llvm/)
set(LLVM_PATH ${CMAKE_CURRENT_LIST_DIR}/build/third_party/llvm/)

add_subdirectory(third_party/omr)
add_subdirectory(third_party/llvm)

add_subdirectory(lljb)
add_subdirectory(test)