#pragma once

#include <vector>

#include "obstacle.h"
#include "sensor.h"

namespace simulation {
	struct Vehicle : Drawable {
		Vehicle()
			: Drawable(Transform(), 0.f), turning_force(0.f), speed(0.f), velocity(0.f), acceleration(0.f) { }

		Vehicle(const Transform& transform, const vec4& colour, float turning_force, float speed)
			: Drawable(transform, colour), turning_force(turning_force), speed(speed), velocity(0.f), acceleration(0.f) 
		{


			float sz = utils::gen_random(100.f, 200.f);

			Transform left_sensor_transform = Transform{vec3{0.f}, vec3{sz, sz, 0.f}, 45.f};
			left_sensor = Sensor(left_sensor_transform, {1.f, 1.f, 0.f, 1.f}, {1.f, 1.f});

			Transform right_sensor_transform = Transform{vec3{0.f}, vec3{sz, sz, 0.f}, -45.f};
			right_sensor = Sensor(right_sensor_transform, {1.f, 1.f, 0.f, 1.f}, {1.f, -1.f});
		}

		std::vector<std::string> string_attribs();

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw_sensors();
		void draw();
		void destroy();

		Sensor left_sensor;
		Sensor right_sensor;

		// Physics
		float turning_force;
		float speed;
		vec2 velocity;
		vec2 acceleration;

		std::vector<vec2> other_vehicle_locations;
	};
}