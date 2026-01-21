
#include "common.hpp"

void Release(Context& ctx)
{
	ctx.res.Release<Shader>([](auto& r) { UnloadShader(r); });

	CloseWindow();
}
