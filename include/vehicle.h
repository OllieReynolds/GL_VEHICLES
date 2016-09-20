#pragma once

#include "obstacle.h"
#include "sensor.h"

namespace simulation {
	struct Vehicle : Drawable {
		Vehicle() {}
		Vehicle(const Sensor& sl, const Sensor& sr, const vec2& position = vec2{1.f},
			float speed = 1.f, float turning_force = 1.f);

		void move();
		bool test_sensor_activity(const Sensor& s);

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		void set_obstacle_ptr(Obstacle& obs) { obstacle = &obs; }

		Obstacle* obstacle;

		mat4 model;

		Sensor left_sensor;
		Sensor right_sensor;

		vec2 position;

		float speed;
		float turning_force;

		vec2 velocity;
		vec2 acceleration;
	};
}