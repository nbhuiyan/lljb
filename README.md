# LLJB

LLVM + JitBuilder = LLJB

[![Build Status](https://travis-ci.org/nbhuiyan/lljb.svg?branch=master)](https://travis-ci.org/nbhuiyan/lljb)

# About

LLJB takes LLVM IR modules and JIT-compiles them using the
Eclipse OMR project's JitBuilder library.

# Build and run

## Requirements

* CMake 3.5 and newer
* clang 8.0
* llvm 8.0
* ninja
* [Eclipse OMR Build requirements](https://github.com/eclipse/omr/blob/master/doc/BuildingWithCMake.md#requirements)

## Build instructions

### Clone this repo and submodules

```
git clone --recursive https://github.com/nbhuiyan/lljb.git
```

### Create and navigate to build directory

```
mkdir build && cd build
```

### Configure

```
cmake -G Ninja <path-to-lljb>
```

### Build

```
ninja -j<num-cpu-threads>
```

### Test

```
./test/lljb_run test/cpp/<program-name>.ll
```
