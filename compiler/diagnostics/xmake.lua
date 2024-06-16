target("swallow_compiler_diagnostics")
    set_kind("static")
    set_languages("c++20")

    add_files("*.cpp")
    add_includedirs("..")
    add_ldflags("clang::-stdlib=libc++")
    add_cxxflags("clang::-stdlib=libc++")