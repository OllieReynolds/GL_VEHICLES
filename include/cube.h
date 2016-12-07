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

		void init(const mat4& projection_matrix);
		void update(const maths::vec2& cursor_pos);
		void draw(const mat4& view_matrix, const mat4& c);
		void destroy();
	};
}