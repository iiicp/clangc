SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(test_file_manager FileManager_test.cc)
target_link_libraries(test_file_manager PRIVATE GTest::GTest gtest_main ClangC LLVMSupport)

add_test(add_gtests test_file_manager)