set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", {outputdir = "."})

add_requires("stx")

includes("src/diagnostics")
target("Swallow")
    set_kind("binary")
    add_files("src/*.cpp")

    add_packages("STX")
    add_deps("diagnostics")