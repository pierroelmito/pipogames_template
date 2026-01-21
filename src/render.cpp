
#include "common.hpp"

namespace Render {

void Do(Context& /*ctx*/, const RenderParams /*p*/)
{
	BeginDrawing();
	ClearBackground(PINK);
	EndDrawing();
}

} // namespace Render
