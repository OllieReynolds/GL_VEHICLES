#pragma once

#include <vector>

#include "line_renderer.h"
#include "drawable.h"
#include "global_data.h"
#include "utils.h"

namespace simulation {
	struct Vehicle : Drawable {
		Vehicle() : Drawable(Transform(), 0.f), turning_force(0.f), speed(0.f), velocity(0.f), acceleration(0.f) { }

		Vehicle(const Transform& transform, const vec4& colour, float turning_force, float speed)
			: Drawable(transform, colour), turning_force(turning_force), speed(speed), velocity(0.f), acceleration(0.f), detected(false) { }

		std::vector<std::string> string_attribs();

		void init(const mat4& projection_matrix);
		void update(const maths::vec2& cursor_pos);
		void draw(const mat4& view_matrix, const mat4& projection_matrix);
		void destroy();

		bool detected;
	
		float turning_force;
		float speed;

		vec2 velocity;
		vec2 acceleration;
		vec2 direction;

		std::vector<vec2> other_vehicle_locations;
	};
}