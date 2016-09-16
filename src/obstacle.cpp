#include "..\include\obstacle.h"

namespace simulation {
	void Obstacle::move(const vec2& cursor_position) {
		position = cursor_position;
	}
}