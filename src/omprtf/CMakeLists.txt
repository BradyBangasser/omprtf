add_executable(omprtf main.cpp)

target_include_directories(omprtf
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src/cmeta
    ${CMAKE_SOURCE_DIR}/src/profiler/src
)

target_link_libraries(omprtf PRIVATE cmeta logging protool LLVMSupport LLVMCore LLVMIRReader)
