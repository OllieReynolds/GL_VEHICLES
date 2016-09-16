#pragma once

#include "obstacle.h"
#include "sensor.h"

namespace simulation {
	struct Vehicle {
		Vehicle(const Sensor& left_sensor = {}, const Sensor& right_sensor = {}, const vec2& position = vec2{1.f},
			float speed = 1.f, float turning_force = 1.f);

		void move(const Obstacle& obstacle);
		bool test_sensor_activity(const Sensor& s, const Obstacle& obstacle);


		Sensor left_sensor;
		Sensor right_sensor;

		vec2 position;

		float speed;
		float turning_force;

		vec2 velocity;
		vec2 acceleration;
	};
}