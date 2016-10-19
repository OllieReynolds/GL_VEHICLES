#pragma once

#include <vector>

#include "obstacle.h"
#include "sensor.h"

namespace simulation {
	struct Vehicle : Drawable {
		Vehicle()
			: Drawable(Transform(), 0.f), turning_force(0.f), velocity(0.f), acceleration(0.f) { }

		Vehicle(const Transform& transform, const vec4& colour, float turning_force)
			: Drawable(transform, colour), turning_force(turning_force), velocity(0.f), acceleration(0.f) 
		{
			vec2 left_sensor_position  = vec2{0.f, 0.f};
			vec2 left_sensor_size      = vec2{250.f, 250.f};
			float left_sensor_rotation = 45.f;
			Transform left_sensor_transform = Transform(left_sensor_position, left_sensor_size, left_sensor_rotation);
			left_sensor = Sensor(left_sensor_transform, {1.f, 1.f, 0.f, 1.f}, {1.f, 1.f});

			vec2 right_sensor_position  = vec2{0.f, 0.f};
			vec2 right_sensor_size      = vec2{250.f, 250.f};
			float right_sensor_rotation = -45.f;
			Transform right_sensor_transform = Transform(right_sensor_position, right_sensor_size, right_sensor_rotation);
			right_sensor = Sensor(right_sensor_transform, {1.f, 1.f, 0.f, 1.f}, {1.f, -1.f});
		}

		std::vector<std::string> string_attribs();

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		Sensor left_sensor;
		Sensor right_sensor;

		// Physics
		float turning_force;
		vec2 velocity;
		vec2 acceleration;
	};
}