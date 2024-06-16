set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

includes("../base", "./utils")

add_requires("llvm", { system = true })

target("swc")
    set_kind("static")
    set_languages("c++20")

    add_files("*.cpp", "**/*.cpp")
    add_includedirs(".", "ast", "type", "lexer", "parser", "diagnostics", "utils", "ir")
    add_ldflags("clang::-stdlib=libc++")
    add_cxxflags("clang::-stdlib=libc++")
    
    add_packages("llvm")
    add_deps("base", "libswallow_utils")