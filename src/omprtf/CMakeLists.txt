set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(HASH_FUNCTION t1ha0_ia32aes_avx2)

add_compile_definitions(HASH_FUNCTION_${HASH_FUNCTION})
add_compile_definitions(HASH_FUNCTION=${HASH_FUNCTION})

add_executable(omprtf main.cpp)

target_include_directories(omprtf
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/cmeta
    ${CMAKE_SOURCE_DIR}/src/profiler/src
    ${CMAKE_SOURCE_DIR}/src/mreader
    ${CMAKE_SOURCE_DIR}/src/opt_pass
)

target_link_libraries(omprtf PRIVATE cmeta logging protool mreader libprofiler LLVMSupport LLVMCore LLVMIRReader opt_pass)
