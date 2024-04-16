includes("../compiler")

target("swi")
    set_kind("binary")
    set_languages("c++20")
    add_files("*.cpp")
    add_deps("swc")