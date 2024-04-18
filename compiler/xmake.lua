set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

includes("../base")

add_packages("libc++")

target("swc")
    set_kind("binary")
    set_languages("c++20")
    
    add_files("*.cpp", "**/*.cpp")
    add_includedirs(".", "ast", "type", "lexer", "parser", "diagnostics", "utils")
    add_cxxflags("clang::-stdlib=libc++")
    add_deps("base")
    add_packages("libc++")

    after_build(function (_)
        os.run("sh after_build.sh")
    end)