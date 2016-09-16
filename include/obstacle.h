#pragma once

#include "maths.h"

namespace simulation {
	using namespace maths;

	struct Obstacle {
		vec4 colour;
		vec2 position;
		vec2 size;

		void move(const vec2& cursor_position);
	};
}