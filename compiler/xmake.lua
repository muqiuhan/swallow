set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

target("swc")
    set_kind("static")
    set_languages("c++20")

    add_files("*.cpp", "**/*.cpp")
    add_includedirs(".")
    add_ldflags("clang::-stdlib=libc++")
    add_cxxflags("clang::-stdlib=libc++")