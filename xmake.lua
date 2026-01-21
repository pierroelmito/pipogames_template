
add_rules("mode.debug", "mode.release", "mode.profile")

if not is_mode("debug") then
	set_policy("build.optimization.lto", true)
end

rule("shader")
	set_extensions(".args")
	on_buildcmd_file(function (target, batchcmds, sourcefile, opt)
		batchcmds:mkdir(target:targetdir())
		local dir = path.directory(sourcefile)
		local src = path.filename(sourcefile)
		local dst = 'data/shaders/' .. path.basename(sourcefile) .. ".glsl"
		local targetfile = '../../' .. dst
		local global = path.join(dir, "global.shader")
		local common = path.join(dir, "common.glsl")
		batchcmds:show_progress(opt.progress, "${color.build.object}shader %s", targetfile)
		batchcmds:vrunv('/bin/sh', {'-c', "cd " .. dir .. "; mcpp -k `cat " .. src .. "` -P global.shader | grep -v '^$$' > " .. targetfile})
		batchcmds:add_depfiles(sourcefile, global, common)
	end)

target("raylib_sdl")
	set_kind("static")
	add_files(
		"ext/raylib/src/raudio.c",
		"ext/raylib/src/rcore.c",
		"ext/raylib/src/rmodels.c",
		"ext/raylib/src/rshapes.c",
		"ext/raylib/src/rtext.c",
		"ext/raylib/src/rtextures.c",
		"ext/raylib/src/utils.c"
	)
	add_includedirs("ext/raylib/src", "/usr/include/SDL2")
	add_defines("PLATFORM_DESKTOP_SDL", "GRAPHICS_API_OPENGL_33")
	if is_mode("debug") then
		add_defines("DEBUG")
	end

target("main")
	set_policy("build.warning", true)
	set_warnings("all", "extra")
	add_rules("shader")
	set_kind("binary")
	add_files("src/*.cpp")
	add_files("src/shaders/*.args")
	add_cxflags("-std=c++20")
	add_deps("raylib_sdl")
	add_includedirs("ext/raylib/src")
	add_links("SDL2")
	set_rundir(".")
	if is_mode("debug") then
		add_defines("DEBUG")
	end

