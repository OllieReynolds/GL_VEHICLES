#pragma once

#include "drawable.h"
#include "maths.h"
#include "vehicle.h"

namespace simulation {
	using namespace maths;

	class Obstacle : public Drawable {
	public:
		Obstacle()
			: Drawable(Transform(), 0.f) { }

		Obstacle(const Transform& transform, const vec4& colour = 0.f) 
			: Drawable(transform, colour) { }

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		void move(const vec2& cursor_position);
	};
}