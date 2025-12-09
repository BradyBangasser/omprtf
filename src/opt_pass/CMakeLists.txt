message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(opt_pass SHARED pass.cpp)

llvm_map_components_to_libnames(LLVM_LIBS core support passes)

target_link_libraries(opt_pass PRIVATE ${LLVM_LIBS})

set_target_properties(opt_pass PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set(PASS_SO_ABSPATH "${CMAKE_BINARY_DIR}/libpass.so")

set(RUN_SCRIPT ${CMAKE_BINARY_DIR}/run_opt_pass.sh)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/run.sh.in
    ${RUN_SCRIPT}
    @ONLY
)

add_custom_command(
    TARGET opt_pass POST_BUILD
    COMMAND chmod +x ${RUN_SCRIPT}
)
