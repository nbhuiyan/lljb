###############################################################################
# Copyright (c) 2019, 2019 IBM Corp. and others
#
# This program and the accompanying materials are made available under
# the terms of the Eclipse Public License 2.0 which accompanies this
# distribution and is available at http://eclipse.org/legal/epl-2.0
# or the Apache License, Version 2.0 which accompanies this distribution
# and is available at https://www.apache.org/licenses/LICENSE-2.0.
#
# This Source Code may also be made available under the following Secondary
# Licenses when the conditions for such availability set forth in the
# Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
# version 2 with the GNU Classpath Exception [1] and GNU General Public
# License, version 2 with the OpenJDK Assembly Exception [2].
#
# [1] https://www.gnu.org/software/classpath/license.html
# [2] http://openjdk.java.net/legal/assembly-exception.html
#
# SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
#############################################################################

# we need clang to generate IR from C++
find_program(CLANG_EXECUTABLE  NAMES clang)

if (NOT CLANG_EXECUTABLE)
    message(SEND_ERROR "Could not find clang")
endif()

# Generate llvm bitcode files from C++ using clang
function(generate_module_from_cpp src)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/${src}.ll
        COMMAND
            ${CLANG_EXECUTABLE} -S -emit-llvm -std=c++0x -O0 -o ${CMAKE_CURRENT_BINARY_DIR}/${src}.ll ${CMAKE_CURRENT_SOURCE_DIR}/${src}.cpp
        MAIN_DEPENDENCY
            ${CMAKE_CURRENT_SOURCE_DIR}/${src}.cpp
    )
    add_custom_target(generate_module_from_cpp_${src} ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${src}.ll
    )

endfunction(generate_module_from_cpp)

generate_module_from_cpp(add)
generate_module_from_cpp(sub)
generate_module_from_cpp(mul)
generate_module_from_cpp(div)
generate_module_from_cpp(conditional)
generate_module_from_cpp(function_call)
generate_module_from_cpp(function_params)
generate_module_from_cpp(floats)
generate_module_from_cpp(doubles)
generate_module_from_cpp(globals)
generate_module_from_cpp(print_string)
generate_module_from_cpp(loops)
generate_module_from_cpp(structs)
generate_module_from_cpp(arrays)
generate_module_from_cpp(time)
generate_module_from_cpp(mandelbrot)
generate_module_from_cpp(ternary)
generate_module_from_cpp(md5)
generate_module_from_cpp(unions)
