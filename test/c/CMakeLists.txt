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
