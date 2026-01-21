
#include "common.hpp"

bool Init(Context& ctx, std::span<char*> args)
{
#ifndef DEBUG
	SetTraceLogLevel(LOG_WARNING);
#endif
	for (auto* arg : args)
		TraceLog(LOG_INFO, "arg: %s", arg);
	InitWindow(1280, 720, "main");
	SetWindowState(FLAG_VSYNC_HINT);
	SetTargetFPS(100);
	SetExitKey(KEY_NULL);
	DisableCursor();
	using LocList = const std::vector<std::pair<size_t, const char*>>;
	LocList instancing = { { SHADER_LOC_MATRIX_MODEL, "instanceTransform" } };
	ctx.res.Add(ctx.shaders.Default) = rlt::MakeShader("default", "default");
	ctx.res.Add(ctx.shaders.Instancing) = rlt::MakeShader("instancing", "default", {}, instancing);
	return true;
}
