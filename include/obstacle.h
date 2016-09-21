#pragma once

#include "drawable.h"
#include "maths.h"

namespace simulation {
	using namespace maths;

	class Obstacle : public Drawable {
	public:
		Obstacle(const vec4& colour = {1.f}, const vec2& position = {1.f}, const vec2& size = {1.f}, 
			const float rotation = 0.f);

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		void move(const vec2& cursor_position);

		vec4 colour;
		vec2 position;
		vec2 size;
		float rotation;
	};
}