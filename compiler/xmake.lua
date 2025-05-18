add_requires("spdlog")

target("swc")
    set_kind("static")
    set_languages("c++20")

    add_files("*.cpp", "**/*.cpp")
    add_packages("spdlog")
    add_includedirs(".")

    before_build(function () 
        local command = "flex -o $(projectdir)/compiler/lexer/flex_lexer.cpp $(projectdir)/compiler/lexer/lexer.l"
        printf("[ RUN]: %s\n", command)
        os.run(command)
    end)

    before_build(function ()
        local command = "bison -o $(projectdir)/compiler/parser/bison_parser.cpp -d $(projectdir)/compiler/parser/parser.y"
        printf("[ RUN]: %s\n", command)
        os.run(command)
    end)