set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

includes("../base")

target("swc")
    set_kind("binary")
    set_languages("c++20")
    
    add_cxxflags("-stdlib=libc++", {tools = "clang"})
    add_links("c++", {tools = "clang"})
    add_files("*.cpp", "**/*.cpp")
    add_includedirs(".", "ast", "type", "lexer", "parser", "diagnostics", "utils")
    add_deps("base")