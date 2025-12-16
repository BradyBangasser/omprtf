cmake_minimum_required(VERSION 3.10)
project(omprtf C CXX)
set(LLVM_LINK_LLVM_DYLIB ON)
execute_process(
    COMMAND git submodule update --init --recursive
    # Run the command from the root of the source directory
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    # Store the result code
    RESULT_VARIABLE GIT_SUBMODULE_RESULT
    # Display the output to the user
    OUTPUT_VARIABLE GIT_SUBMODULE_OUTPUT
    ERROR_VARIABLE  GIT_SUBMODULE_ERROR
  )

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR})
endforeach()

set(CMAKE_TEST_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
file(MAKE_DIRECTORY ${CMAKE_TEST_OUTPUT_DIRECTORY})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Make sure LLVM is found
find_package(LLVM REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Add subdirectories
add_subdirectory(src/logging)
add_subdirectory(src/cmeta)
add_subdirectory(src/opt_pass)
add_subdirectory(src/profiler)
add_subdirectory(src/mreader)
add_subdirectory(src/omprtf)
add_subdirectory(eval)

