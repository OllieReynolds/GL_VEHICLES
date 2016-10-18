#pragma once

#include <math.h>

#include "drawable.h"
#include "maths.h"
#include "utils.h"

namespace simulation {
	using namespace maths;
	
	extern const float PI;

	// Very ugly, move the attribs on their own back to sensor attribs and do the GL wrangling without the struct
	/*struct Sensor_Attribs {
		Sensor_Attribs() { }

		Sensor_Attribs(const vec4& colour, const vec2& position, const vec2& heading, float theta, float radius) 
			: colour(colour), position(position), radius(radius)
		{
			float heading_angle = atan2(heading.y, heading.x) * 180.f / PI;

			float start_arm_angle_deg = heading_angle - theta * 0.5f;
			float end_arm_angle_deg = heading_angle + theta * 0.5f;

			float start_arm_angle_rad = start_arm_angle_deg * PI / 180.f;
			float end_arm_angle_rad = end_arm_angle_deg * PI / 180.f;

			start = {cos(start_arm_angle_rad), sin(start_arm_angle_rad)};
			end = {cos(end_arm_angle_rad), sin(end_arm_angle_rad)};
		}

		void update_beam_headings(const mat4& model) {
			start = mult(model, vec4{start.x, start.y, 0.f, 1.f}).XY();
			end = mult(model, vec4{end.x, end.y, 0.f, 1.f}).XY();
		}

		vec4 colour;
		vec2 position;
		vec2 start;
		vec2 end;
		float radius;
	};*/

	class Sensor : public Drawable {
	public:
		Sensor() 
			: Drawable(Transform(), 0.f), detected_object(false) { }

		Sensor(const Transform& transform, const vec4& colour, const vec2& heading, const float angle) 
			: Drawable(transform, colour), heading(heading), angle(angle), detected_object(false) { }

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		std::pair<vec2, vec2> get_sensor_arms_AB();
		void scan(const vec2& position);

		float angle;
		vec2 heading;
		mat4 parent_model;
		bool detected_object;
	};
}