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
			: Drawable(Transform(), 0.f), detected_object(false) { }

		Sensor(const Transform& transform, const vec4& colour, const vec2& heading) 
			: Drawable(transform, colour), heading(heading), detected_object(false) { }

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		std::pair<vec2, vec2> get_sensor_arms();
		void scan(const vec2& position);

		vec2 heading;
		mat4 parent_model;
		Transform parent_transform;
		bool detected_object;
	};
}