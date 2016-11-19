#pragma once

#include "drawable.h"
#include "maths.h"
#include "vehicle.h"

namespace simulation {
	using namespace maths;

	class Cube : public Drawable {
	public:
		Cube()
			: Drawable({0.f, 0.f, 0.f}, 0.f) {
		}

		Cube(const Transform& t, const vec4& colour = 0.f)
			: Drawable(t, colour) {
		}

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();
	};
}