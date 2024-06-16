target("swallow_compiler_lexer")
    set_kind("static")
    set_languages("c++20")

    add_files("*.cpp")
    add_includedirs("..")
    add_ldflags("clang::-stdlib=libc++")
    add_cxxflags("clang::-stdlib=libc++")

    before_build(function () 
        local command = "flex -o $(projectdir)/compiler/lexer/flex_lexer.cpp $(projectdir)/compiler/lexer/lexer.l"
        printf("[ RUN]: %s\n", command)
        os.run(command)
    end)