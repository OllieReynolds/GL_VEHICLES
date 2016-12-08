#pragma once

#include <vector>

#include "draw_line.h"
#include "obstacle.h"
#include "sensor.h"
#include "global_data.h"

namespace simulation {
	struct Vehicle : Drawable {
		Vehicle() : Drawable(Transform(), 0.f), turning_force(0.f), speed(0.f), velocity(0.f), acceleration(0.f) { }

		Vehicle(const Transform& transform, const vec4& colour, float turning_force, float speed, Draw_Line* draw_line)
			: Drawable(transform, colour), turning_force(turning_force), speed(speed), velocity(0.f), acceleration(0.f), detected(false)
		{
			float sz = 50.f;

			Transform left_sensor_transform = Transform{vec3{0.f}, vec3{sz, 0.f, sz}, 45.f};
			left_sensor = Sensor(left_sensor_transform, {1.f, 1.f, 0.f, 1.f}, {1.f, 1.f});

			Transform right_sensor_transform = Transform{vec3{0.f}, vec3{sz, 0.f, sz}, -45.f};
			right_sensor = Sensor(right_sensor_transform, {1.f, 1.f, 0.f, 1.f}, {1.f, -1.f});
		}

		std::vector<std::string> string_attribs();

		void init(const mat4& projection_matrix);
		void update(const maths::vec2& cursor_pos);
		void draw_sensors(const mat4& view_matrix, const mat4& projection_matrix);
		void draw(const mat4& view_matrix, const mat4& projection_matrix);
		void destroy();

		Draw_Line* dl;

		Sensor left_sensor;
		Sensor right_sensor;

		bool detected;

		vec2 direction;

		// Physics
		float turning_force;
		float speed;
		vec2 velocity;
		vec2 acceleration;

		std::vector<vec2> other_vehicle_locations;
	};
}