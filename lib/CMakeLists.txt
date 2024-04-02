add_subdirectory(Basic)
add_subdirectory(Lex)
add_subdirectory(Driver)

set(clang_c_src
        ${clangcBasic}
        ${clangcLex}
        ${clangcDriver}
        )
add_library(ClangC STATIC ${clang_c_src})