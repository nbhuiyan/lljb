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

# we need clang to generate IR from C
find_program(CLANG_EXECUTABLE  NAMES clang)

if (NOT CLANG_EXECUTABLE)
    message(SEND_ERROR "Could not find clang")
endif()

# Generate llvm bitcode files from C using clang
function(generate_module_from_c src)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/${src}.ll
        COMMAND
            ${CLANG_EXECUTABLE} -S -emit-llvm -O0 -o ${CMAKE_CURRENT_BINARY_DIR}/${src}.ll ${CMAKE_CURRENT_SOURCE_DIR}/${src}.c
        MAIN_DEPENDENCY
            ${CMAKE_CURRENT_SOURCE_DIR}/${src}.c
    )
    add_custom_target(generate_module_from_c_${src} ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${src}.ll
    )

endfunction(generate_module_from_c)

generate_module_from_c(mandelbrot)
