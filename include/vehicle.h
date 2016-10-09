#pragma once

#include "obstacle.h"
#include "sensor.h"

namespace simulation {
	struct Vehicle : Drawable {
		Vehicle(const vec2& position = vec2{0.f}, const vec2& size = vec2{0.f}, const vec2& sensor_offset = vec2{0.f}, 
			float rotation = 0.f, float speed = 0.f, float turning_force = 0.f);

		void move();
		bool test_sensor_activity(const Sensor& s);

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		void set_obstacle_ptr(Obstacle& obs) { obstacle = &obs; }

		Obstacle* obstacle;

		Sensor left_sensor;
		Sensor right_sensor;
		vec2 sensor_offset;

		vec2 position;
		vec2 size;
		float rotation;

		float speed;
		float turning_force;

		vec2 velocity;
		vec2 acceleration;
	};
}