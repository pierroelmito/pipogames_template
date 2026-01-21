
#include "common.hpp"

namespace Update {

bool Do(Context& /*ctx*/, const UpdateParams /*p*/)
{
	if (WindowShouldClose())
		return false;
	return true;
}

} // namespace Update
