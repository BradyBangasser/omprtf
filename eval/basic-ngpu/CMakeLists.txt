get_filename_component(SUBDIR_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)

set(OUTPUT_DIR "eval")

set(OUTPUT_BASE_NAME "${CMAKE_BINARY_DIR}/${OUTPUT_DIR}/${SUBDIR_NAME}")

find_package(OpenMP)

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}")

add_executable(${SUBDIR_NAME}.out ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

set_target_properties(${SUBDIR_NAME}.out PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    )

target_compile_options(${SUBDIR_NAME}.out PRIVATE -g)
target_link_options(${SUBDIR_NAME}.out PRIVATE -g)
target_link_libraries(${SUBDIR_NAME}.out PRIVATE OpenMP::OpenMP_C)

add_custom_command(
        OUTPUT ${OUTPUT_BASE_NAME}.bc
        COMMAND ${CMAKE_C_COMPILER} -c -o ${OUTPUT_BASE_NAME}.bc ${CMAKE_CURRENT_SOURCE_DIR}/main.c
                -emit-llvm ${OpenMP_C_FLAGS}
        DEPENDS main.c
        COMMENT "Generating LLVM Bitcode (${OUTPUT_BASE_NAME}.bc)"
        VERBATIM
    )

add_custom_command(
        OUTPUT ${OUTPUT_BASE_NAME}.ll
        COMMAND ${CMAKE_C_COMPILER} -o ${OUTPUT_BASE_NAME}.ll ${CMAKE_CURRENT_SOURCE_DIR}/main.c
                -S -emit-llvm ${OpenMP_C_FLAGS}
        DEPENDS main.c
        COMMENT "Generating LLVM IR (${OUTPUT_BASE_NAME}.ll)"
        VERBATIM
    )

add_custom_target(${SUBDIR_NAME}_generate_ir_artifacts ALL
        DEPENDS ${OUTPUT_BASE_NAME}.bc ${OUTPUT_BASE_NAME}.ll
    )
