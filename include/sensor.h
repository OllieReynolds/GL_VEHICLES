#pragma once

#include <math.h>

#include "drawable.h"
#include "maths.h"
#include "utils.h"

namespace simulation {
	using namespace maths;
	
	class Sensor : public Drawable {
	public:
		Sensor() 
			: Drawable({0.f, 0.f, 0.f}, 0.f), detected_object(0) 
		{ 
		}

		Sensor(const Transform& t, const vec4& colour, const vec2& heading) 
			: Drawable(t, colour), heading(heading), detected_object(0) { }

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		std::pair<vec2, vec2> get_sensor_arms();
		bool scan(const std::vector<vec2>& points);

		vec2 heading;
		mat4 parent_model;
		Transform parent_transform;

		bool detected_object;
	};
}