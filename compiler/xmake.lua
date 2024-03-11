set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

includes("../stdlib")

target("swc")
    set_kind("binary")
    set_languages("c++20")
    
    add_cxxflags("clang::-stdlib=libc++")
    add_files("*.cpp", "**/*.cpp")
    add_includedirs(".", "ast", "type", "lexer", "parser", "diagnostics", "utils")

    add_links("c++")

    before_build(function (target)
      os.run("flex -o $(scriptdir)/lexer/flex_lexer.cpp $(scriptdir)/lexer/lexer.l")
      os.run("bison -o $(scriptdir)/parser/bison_parser.cpp -d $(scriptdir)/parser/parser.y")
    end)

    after_build(function (_)
        os.run("sh after_build.sh")
    end)

    add_deps("stdlib")