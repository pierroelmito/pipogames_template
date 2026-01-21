#pragma once

#include "mc/rltools.hpp"

struct Resources {
	rlt::Resources<Shader> res {};
	struct : public rlt::Dict<Shader> {
		ID Default {};
		ID Instancing {};
	} shaders;
};

struct Context : public Resources { };

bool Init(Context& ctx, std::span<char*> args);
void Release(Context& ctx);

namespace Update {
struct UpdateParams {
	const float dt {};
};
bool Do(Context& ctx, const UpdateParams p);
} // namespace Update

namespace Render {
struct RenderParams {
	const int w {};
	const int h {};
};
void Do(Context& ctx, const RenderParams p);
} // namespace Render
