#pragma once

#include "maths.h"
#include "drawable.h"

namespace simulation {

	using namespace maths;

	class Boundary : public Drawable {
	public:
		Boundary()
			: Drawable(Transform(), 0.f) { }

		Boundary(const Transform& transform, const vec4& colour)
			: Drawable(transform, colour) { }

		~Boundary() { }

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();
	};
}