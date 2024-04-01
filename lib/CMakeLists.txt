add_subdirectory(Basic)
add_subdirectory(Lex)

set(c_clang_src
        ${cclangBasic}
        ${cclangLex}
        )
add_library(ClangC STATIC ${c_clang_src})