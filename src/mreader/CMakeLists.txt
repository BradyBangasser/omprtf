add_library(mreader SHARED getfileinfo.cpp)
set_target_properties(mreader PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(mreader PRIVATE logging PUBLIC LLVMCore LLVMSupport LLVMObject LLVMBitReader LLVMBitstreamReader LLVMDebugInfoDWARF)
target_include_directories(mreader PRIVATE ${CMAKE_SOURCE_DIR}/src/logging ${LLVM_INCLUDE_DIRS})

add_executable(readmeta tool.c)
target_link_libraries(readmeta PRIVATE logging mreader)
target_include_directories(readmeta PRIVATE ${CMAKE_SOURCE_DIR}/src/logging ${CMAKE_SOURCE_DIR}/src/mreader)
