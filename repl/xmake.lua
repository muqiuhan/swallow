includes("../compiler")

target("swi")
    set_kind("static")
    set_languages("c++20")
    add_files("*.cpp")
    add_deps("swc")