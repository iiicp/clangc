add_subdirectory(Basic)
add_subdirectory(Lex)

set(c_clang_src
        ${cclangBasic}
        ${cclangLex}
        )
add_library(CClang STATIC ${c_clang_src})