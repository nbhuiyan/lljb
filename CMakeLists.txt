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

add_subdirectory(third_party/omr)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(ENV{PATH} "/usr/local/opt/llvm\@8/bin/:${PATH}")
endif()

find_package(LLVM 8 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})

add_subdirectory(lljb)
add_subdirectory(test)
