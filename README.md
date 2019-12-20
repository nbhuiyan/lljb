<!--
Copyright (c) 2019, 2019 IBM Corp. and others

This program and the accompanying materials are made available under
the terms of the Eclipse Public License 2.0 which accompanies this
distribution and is available at https://www.eclipse.org/legal/epl-2.0/
or the Apache License, Version 2.0 which accompanies this distribution and
is available at https://www.apache.org/licenses/LICENSE-2.0.

This Source Code may also be made available under the following
Secondary Licenses when the conditions for such availability set
forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
General Public License, version 2 with the GNU Classpath 
Exception [1] and GNU General Public License, version 2 with the
OpenJDK Assembly Exception [2].

[1] https://www.gnu.org/software/classpath/license.html
[2] http://openjdk.java.net/legal/assembly-exception.html

SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
-->

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
ninja
```

### Test

```
./test/lljb_run test/cpp/<program-name>.ll
```
