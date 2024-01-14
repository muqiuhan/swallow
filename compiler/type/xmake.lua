target("type")
    set_kind("shared")
    set_languages("c++20")
    add_files("src/**.cpp")
    add_includedirs("include", "../utils/optional", "../utils/panic")

    after_build(function (target)
        os.run("xmake format")
    end)