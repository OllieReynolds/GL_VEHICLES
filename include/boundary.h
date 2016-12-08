#pragma once

#include "global_data.h"
#include "maths.h"
#include "drawable.h"

namespace simulation {

	using namespace maths;

	class Boundary : public Drawable {
	public:
		Boundary() : Drawable(Transform(), utils::colour::black) { }
		Boundary(const Transform& t, const vec4& colour) : Drawable(t, colour) { }
		~Boundary() { }

		void init(const mat4& projection_matrix);
		void update(const maths::vec2& cursor_pos);
		void draw(const mat4& view_matrix, const mat4& projection_matrix);
		void destroy();
	};
}