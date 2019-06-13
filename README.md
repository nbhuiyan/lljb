# LLJB

LLVM + JitBuilder = LLJB

*new project, so check back for more documentation later...*

# About

LLJB takes LLVM IR modules and JIT-compiles them using the
Eclipse OMR project's JitBuilder library.

# Build and run

## Requirements

* CMake 3.5 and newer
* clang 8.0
* [LLVM build requirements](https://llvm.org/docs/GettingStarted.html#requirements)
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
cmake <path-to-lljb>
```

### Build

```
make -j<num-cpu-threads>
```

### Test

```
./test/lljb_run test/cpp/<program-name>.ll
```
