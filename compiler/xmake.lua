set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})

target("swc")
    set_kind("binary")
    set_languages("c++20")
    
    add_files("*.cpp", "**/*.cpp")
    add_includedirs(".", "ast", "type", "lexer", "parser", "diagnostics", "utils")

    before_build(function (target)
      os.run("flex -o $(scriptdir)/lexer/flex_lexer.cpp $(scriptdir)/lexer/lexer.l")
      os.run("bison -o $(scriptdir)/parser/bison_parser.cpp -d $(scriptdir)/parser/parser.y")
    end)

    after_build(function (_)
        os.run("make fmt")
    end)