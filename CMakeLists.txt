cmake_minimum_required(VERSION 3.18)

# set compiler version
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(CClang)

    include_directories("${CMAKE_BINARY_DIR}/include"
            "${CMAKE_BINARY_DIR}/llvm/include"
            "include"
            "llvm/include")
    link_directories("${CMAKE_BINARY_DIR}")

    set(CCLANG_BUILT_STANDALONE 1)
endif ()

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
    message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

set(CLANG_VERSION_STRING "0.1")
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/clang/Basic/Version.inc.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/clang/Basic/Version.inc)

include_directories(BEFORE
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LLVM_INCLUDE_TESTS OFF)
add_subdirectory(llvm)
add_subdirectory(lib)
add_subdirectory(tools/driver)

option(CCLANG_UNIT_TESTS "the CClang unit tests." OFF)

if( CCLANG_UNIT_TESTS )
enable_testing()
add_subdirectory(unittests/clang)
endif ()
