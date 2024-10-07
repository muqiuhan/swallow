set_project("swallow")
set_version("0.0.1")
set_xmakever("2.8.1")

add_rules("mode.debug", "mode.release")

includes("**/xmake.lua")

after_build(function()
    -- imports
    import("core.project.config")
    import("core.project.depend")
    import("core.project.project")
    import("core.base.task")

    -- we should not update it if we are installing xmake package
    if os.getenv("XMAKE_IN_XREPO") then
        return
    end

    function update_compile_commands()
        -- run only once for all xmake process
        local tmpfile = path.join(config.buildir(), ".gens", "rules", "plugin.compile_commands.autoupdate")
        local dependfile = tmpfile .. ".d"
        local lockfile = io.openlock(tmpfile .. ".lock")

        if lockfile:trylock() then
            local outputdir = "."
            local lsp
            local sourcefiles = {}
            for _, target in pairs(project.targets()) do
                table.join2(sourcefiles, target:sourcefiles(), (target:headerfiles()))
                local extraconf = target:extraconf("rules", "plugin.compile_commands.autoupdate")
                if extraconf then
                    outputdir = extraconf.outputdir
                    lsp = extraconf.lsp
                end
            end
            table.sort(sourcefiles)
            depend.on_changed(function()
                -- we use task instead of os.exec("xmake") to avoid the project lock
                local filename = "compile_commands.json"
                local filepath = outputdir and path.join(outputdir, filename) or filename
                task.run("project", { kind = "compile_commands", outputdir = outputdir, lsp = lsp })
                print("[ RUN]: update compile_commands.json")
            end, {
                dependfile = dependfile,
                files = table.join(project.allfiles(), config.filepath()),
                values = sourcefiles
            })
            lockfile:close()
        end
    end

    function update_makefile()
        -- run only once for all xmake process
        local tmpfile = path.join(config.buildir(), ".gens", "rules", "plugin.makefile.autoupdate")
        local dependfile = tmpfile .. ".d"
        local lockfile = io.openlock(tmpfile .. ".lock")

        if lockfile:trylock() then
            local outputdir = "."
            local lsp
            local sourcefiles = {}
            for _, target in pairs(project.targets()) do
                table.join2(sourcefiles, target:sourcefiles(), (target:headerfiles()))
                local extraconf = target:extraconf("rules", "plugin.makefile.autoupdate")
                if extraconf then
                    outputdir = extraconf.outputdir
                    lsp = extraconf.lsp
                end
            end
            table.sort(sourcefiles)
            depend.on_changed(function()
                -- we use task instead of os.exec("xmake") to avoid the project lock
                local filename = "makefile"
                local filepath = outputdir and path.join(outputdir, filename) or filename
                task.run("project", { kind = "makefile", outputdir = outputdir, lsp = lsp })
                print("[ RUN]: update makefile")
            end, {
                dependfile = dependfile,
                files = table.join(project.allfiles(), config.filepath()),
                values = sourcefiles
            })
            lockfile:close()
        end
    end

    function update_cmake()
        -- run only once for all xmake process
        local tmpfile = path.join(config.buildir(), ".gens", "rules", "plugin.cmake.autoupdate")
        local dependfile = tmpfile .. ".d"
        local lockfile = io.openlock(tmpfile .. ".lock")

        if lockfile:trylock() then
            local outputdir = "."
            local lsp
            local sourcefiles = {}
            for _, target in pairs(project.targets()) do
                table.join2(sourcefiles, target:sourcefiles(), (target:headerfiles()))
                local extraconf = target:extraconf("rules", "plugin.cmake.autoupdate")
                if extraconf then
                    outputdir = extraconf.outputdir
                    lsp = extraconf.lsp
                end
            end
            table.sort(sourcefiles)
            depend.on_changed(function()
                -- we use task instead of os.exec("xmake") to avoid the project lock
                local filename = "CMakeLists.txt"
                local filepath = outputdir and path.join(outputdir, filename) or filename
                task.run("project", { kind = "cmake", outputdir = outputdir, lsp = lsp })
                print("[ RUN]: update CMakeLists.txt")
            end, {
                dependfile = dependfile,
                files = table.join(project.allfiles(), config.filepath()),
                values = sourcefiles
            })
            lockfile:close()
        end
    end

    function update_ninja()
        -- run only once for all xmake process
        local tmpfile = path.join(config.buildir(), ".gens", "rules", "plugin.ninja.autoupdate")
        local dependfile = tmpfile .. ".d"
        local lockfile = io.openlock(tmpfile .. ".lock")

        if lockfile:trylock() then
            local outputdir = "."
            local lsp
            local sourcefiles = {}
            for _, target in pairs(project.targets()) do
                table.join2(sourcefiles, target:sourcefiles(), (target:headerfiles()))
                local extraconf = target:extraconf("rules", "plugin.ninja.autoupdate")
                if extraconf then
                    outputdir = extraconf.outputdir
                    lsp = extraconf.lsp
                end
            end
            table.sort(sourcefiles)
            depend.on_changed(function()
                -- we use task instead of os.exec("xmake") to avoid the project lock
                local filename = "build.ninja"
                local filepath = outputdir and path.join(outputdir, filename) or filename
                task.run("project", { kind = "ninja", outputdir = outputdir, lsp = lsp })
                print("[ RUN]: update build.ninja")
            end, {
                dependfile = dependfile,
                files = table.join(project.allfiles(), config.filepath()),
                values = sourcefiles
            })
            lockfile:close()
        end
    end

    update_cmake()
    update_makefile()
    update_compile_commands()
    update_ninja()
end)
