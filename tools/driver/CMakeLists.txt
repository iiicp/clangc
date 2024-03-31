SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(cclang main.cc)

target_link_libraries(cclang
        PRIVATE
        CClang
        LLVMSupport
        LLVMTarget
        )