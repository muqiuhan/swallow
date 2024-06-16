target("swallow_compiler_parser")
    set_kind("static")
    set_languages("c++20")

    add_files("*.cpp")
    add_includedirs("..")
    add_ldflags("clang::-stdlib=libc++")
    add_cxxflags("clang::-stdlib=libc++")

    before_build(function ()
        local command = "bison -o $(projectdir)/compiler/parser/bison_parser.cpp -d $(projectdir)/compiler/parser/parser.y"
        printf("[ RUN]: %s\n", command)
        os.run(command)
    end)