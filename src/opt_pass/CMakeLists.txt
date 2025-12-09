set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

execute_process(
    COMMAND llvm-config --cxxflags
    OUTPUT_VARIABLE LLVM_CXXFLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

string(REPLACE " " ";" LLVM_CXXFLAGS_LIST ${LLVM_CXXFLAGS})
list(REMOVE_ITEM LLVM_CXXFLAGS_LIST "")

execute_process(
    COMMAND llvm-config --system-libs --libs core
    OUTPUT_VARIABLE LLVM_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE " " ";" LLVM_LIBS_LIST ${LLVM_LIBS})

foreach(_lib IN LISTS LLVM_LIBS_LIST)
  string(STRIP "${_lib}" _lib_stripped)
  list(APPEND LLVM_LIBS_CLEAN "${_lib_stripped}")
endforeach()

foreach(_lib IN LISTS LLVM_SYSTEM_LIBS_LIST)
  string(STRIP "${_lib}" _lib_stripped)
  list(APPEND LLVM_SYSTEM_LIBS_CLEAN "${_lib_stripped}")
endforeach()

add_library(opt_pass SHARED pass.cpp)

target_compile_options(opt_pass PRIVATE ${LLVM_CXXFLAGS_LIST})

target_link_libraries(opt_pass PRIVATE ${LLVM_LIBS_CLEAN} ${LLVM_SYSTEM_LIBS_CLEAN})

set_target_properties(opt_pass PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Absolute path where pass.so ends up
set(PASS_SO_ABSPATH "${CMAKE_BINARY_DIR}/libpass.so")

# Generate run.sh from template
set(RUN_SCRIPT ${CMAKE_BINARY_DIR}/run_opt_pass.sh)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/run.sh.in
    ${RUN_SCRIPT}
    @ONLY
)

# Make run.sh executable
add_custom_command(
    TARGET opt_pass POST_BUILD
    COMMAND chmod +x ${RUN_SCRIPT}
)
