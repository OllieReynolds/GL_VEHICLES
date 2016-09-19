#include "..\include\vehicle.h"

namespace simulation {

	Vehicle::Vehicle(const Sensor& left_sensor, const Sensor& right_sensor, const vec2& position, float speed,
		float turning_force)
		: left_sensor(left_sensor), right_sensor(right_sensor), position(position), speed(speed), 
		turning_force(turning_force), velocity(0.f), acceleration(0.f), model({}) {
	}

	bool Vehicle::test_sensor_activity(const Sensor& s, const Obstacle& obstacle) {
		return (maths::point_segment_intersect(obstacle.attribs.position, s.start, s.position, s.end, s.radius));
	}

	void Vehicle::move(const Obstacle& obstacle) {


		if (test_sensor_activity(left_sensor, obstacle)) {
			position.x += speed;
		}

		if (test_sensor_activity(right_sensor, obstacle)) {
			position.x += speed;
		}

		left_sensor.position.x = position.x;
		right_sensor.position.x = position.x;
	}
}