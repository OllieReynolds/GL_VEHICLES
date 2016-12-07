#pragma once

#include <math.h>

#include "drawable.h"
#include "maths.h"
#include "shared.h"
#include "utils.h"

namespace simulation {
	using namespace maths;
	
	class Sensor : public Drawable {
	public:
		Sensor()
			: Drawable({0.f, 0.f, 0.f}, 0.f) { }

		Sensor(const Transform& t, const vec4& colour, const vec2& heading)
			: Drawable(t, colour), heading(heading) { }

		void init(const mat4& projection_matrix);
		void update(const maths::vec2& cursor_pos);
		void draw(const mat4& view_matrix, const mat4& projection_matrix);
		void destroy();

		std::pair<vec2, vec2> get_sensor_arms();
		bool intersects(const std::vector<vec2>& points);
		bool intersects_line(const vec2& a, const vec2& b);

		vec2 heading;
		mat4 parent_model;
		Transform parent_transform;
	};
}