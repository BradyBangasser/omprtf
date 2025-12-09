set(CMAKE_POSITION_INDEPENDENT_CODE ON)

execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXXFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REPLACE " " ";" LLVM_CXXFLAGS_LIST ${LLVM_CXXFLAGS})
list(REMOVE_ITEM LLVM_CXXFLAGS_LIST "")

add_library(opt_pass SHARED pass.cpp)

target_compile_options(opt_pass PRIVATE ${LLVM_CXXFLAGS_LIST})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(opt_pass PRIVATE ${llvm_libs})

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
