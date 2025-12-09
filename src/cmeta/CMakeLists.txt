add_library(cmeta STATIC cmeta.cpp)
add_executable(cmeta.out main.c)

target_include_directories(cmeta
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/logging
        ${LLVM_INCLUDE_DIRS}
)

target_link_libraries(cmeta.out
    PRIVATE
        logging
        cmeta
        LLVM
        LLVMIRReader
)

set_property(TARGET cmeta PROPERTY C_STANDARD 23)
set_property(TARGET cmeta.out PROPERTY C_STANDARD 23)

add_custom_command(
  OUTPUT ${CMAKE_TEST_OUTPUT_DIRECTORY}/cmeta.cpp.ll
  COMMAND clang++ -I${CMAKE_CURRENT_SOURCE_DIR} -I${CMAKE_SOURCE_DIR}/src/logging -I${LLVM_INCLUDE_DIRS} -emit-llvm -S -O0 ${CMAKE_CURRENT_SOURCE_DIR}/cmeta.cpp -o ${CMAKE_TEST_OUTPUT_DIRECTORY}/cmeta.cpp.ll
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/cmeta.cpp
  COMMENT "Compiling ${CMAKE_TEST_OUTPUT_DIRECTORY}/cmeta.cpp.ll"
)

add_custom_target(cmeta.cpp.ll ALL DEPENDS ${CMAKE_TEST_OUTPUT_DIRECTORY}/cmeta.cpp.ll)

add_custom_command(
    OUTPUT run_cmeta
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/cmeta.out ${CMAKE_TEST_OUTPUT_DIRECTORY}/cmeta.cpp.ll
    DEPENDS cmeta.out cmeta.cpp.ll
    COMMENT "Running cmeta.out on ${CMAKE_TEST_OUTPUT_DIRECTORY}/cmeta.cpp.ll"
)

add_custom_target(run_cmeta ALL DEPENDS run_cmeta)
