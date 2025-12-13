set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(HASH_FUNCTION t1ha0_ia32aes_avx2)

add_compile_definitions(HASH_FUNCTION_${HASH_FUNCTION})
add_compile_definitions(HASH_FUNCTION=${HASH_FUNCTION})


add_library(libprofiler SHARED src/tool.cc src/analyze.cc src/symbolizer.cc)
set_target_properties(libprofiler PROPERTIES OUTPUT_NAME "profiler")

target_include_directories(libprofiler
    PRIVATE
        ${LLVM_INCLUDE_DIRS}
    PUBLIC
        hashes/t1ha ${CMAKE_SOURCE_DIR}/src/logging
 )

find_library(LIBDW dw REQUIRED)
target_link_libraries(libprofiler PRIVATE ${LIBDW} logging)

add_library(protool STATIC src/preload.cc)
target_include_directories(protool PUBLIC hashes/t1ha)

add_executable(profile src/main.cc)
target_link_libraries(profile PRIVATE protool)


add_library(t1ha STATIC hashes/t1ha/src/t1ha0.c
                        hashes/t1ha/src/t1ha1.c
                        hashes/t1ha/src/t1ha2.c
                        hashes/t1ha/src/t1ha0_ia32aes_noavx.c
                        hashes/t1ha/src/t1ha0_ia32aes_avx.c
                        hashes/t1ha/src/t1ha0_ia32aes_avx2.c)
set_source_files_properties(hashes/t1ha/src/t1ha0_ia32aes_noavx.c
                            PROPERTIES COMPILE_FLAGS "-mno-avx2 -mno-avx -maes")
set_source_files_properties(hashes/t1ha/src/t1ha0_ia32aes_avx.c
                            PROPERTIES COMPILE_FLAGS "-mno-avx2 -mavx -maes")
set_source_files_properties(hashes/t1ha/src/t1ha0_ia32aes_avx2.c
                            PROPERTIES COMPILE_FLAGS "-mavx -mavx2 -maes")
set_target_properties(t1ha PROPERTIES COMPILE_FLAGS "-O3 -DNDEBUG")
set_target_properties(t1ha PROPERTIES C_STANDARD 11)
set_target_properties(t1ha PROPERTIES C_STANDARD_REQUIRED ON)
set_target_properties(t1ha PROPERTIES C_EXTENSIONS OFF)
set_target_properties(t1ha PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(libprofiler PRIVATE t1ha)
