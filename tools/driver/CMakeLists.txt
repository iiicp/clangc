SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(clangc main.cc)

target_link_libraries(clangc
        PRIVATE
        ClangC
        LLVMSupport
        LLVMTarget
        )