#pragma once

#include "drawable.h"
#include "maths.h"

namespace simulation {
	using namespace maths;

	struct Obstacle_Attribs {
		Obstacle_Attribs(vec4 c = {1.f}, vec2 p = {500.f}, vec2 s = {128.f}) : colour(c), position(p), size(s) {}

		vec4 colour;
		vec2 position;
		vec2 size;
	};

	class Obstacle : public Drawable {
	public:
		Obstacle(Obstacle_Attribs attribs = {}) : attribs(attribs) {}


		Obstacle_Attribs attribs;

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		void move(const vec2& cursor_position);
	};
}