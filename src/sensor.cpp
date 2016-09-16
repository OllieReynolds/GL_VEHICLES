#include "..\include\sensor.h"

namespace simulation {
	const float PI = 3.14159265358979f;

	Sensor::Sensor(const vec4& colour, const vec2& position, const vec2& heading, const float theta, const float radius)
		: colour(colour), position(position), radius(radius) 
	{
		set_sector_arms(heading, theta);
	}

	void Sensor::set_sector_arms(const vec2& heading, float theta) {
		float heading_angle = atan2(heading.y, heading.x) * 180.f / PI;

		float start_arm_angle_deg = heading_angle - theta * 0.5f;
		float end_arm_angle_deg = heading_angle + theta * 0.5f;

		float start_arm_angle_rad = start_arm_angle_deg * PI / 180.f;
		float end_arm_angle_rad = end_arm_angle_deg * PI / 180.f;

		this->start = {cos(start_arm_angle_rad), sin(start_arm_angle_rad)};
		this->end = {cos(end_arm_angle_rad), sin(end_arm_angle_rad)};
	}
}