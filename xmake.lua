add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

add_rules("mode.debug", "mode.release")

includes("**/xmake.lua")