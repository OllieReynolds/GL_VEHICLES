#pragma once

#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "maths.h"
#include "shader.h"
#include "utils.h"

using namespace maths;
using namespace utils;

namespace {
	struct Obstacle {
		vec4 colour;
		vec2 position;
		vec2 size;

		void move(const vec2& cursor_position) {
			position = cursor_position;
		}
	};

	struct Sensor {
		vec4 colour;
		vec2 position;
		vec2 start;
		vec2 end;
		float radius;

		Sensor(const vec4& colour, const vec2& position, const vec2& heading, const float theta, const float radius)
		: colour(colour), position(position), radius(radius) {
			set_sector_arms(heading, theta);
		}

		void set_sector_arms(const vec2& heading, float theta) {
			float heading_angle = atan2(heading.y, heading.x) * 180 / std::_Pi;

			float start_arm_angle_deg = heading_angle - theta * 0.5f;
			float end_arm_angle_deg = heading_angle + theta * 0.5f;

			float start_arm_angle_rad = start_arm_angle_deg * std::_Pi / 180;
			float end_arm_angle_rad = end_arm_angle_deg * std::_Pi / 180;

			this->start = {cos(start_arm_angle_rad), sin(start_arm_angle_rad)};
			this->end = {cos(end_arm_angle_rad), sin(end_arm_angle_rad)};
		}
	};

	struct Vehicle {
		Sensor left_sensor = Sensor(
			{1.f, 1.f, 0.f, 1.f}, // Yellow
			{40.f, 600.f}, // Position, extreme left of screen between the top and middle
			{1.f, 0.5f}, // Heading
			{60.f}, // Radius
			{256.f} // Size of the beam
		);

		Sensor right_sensor = Sensor(
			{1.f, 1.f, 0.f, 1.f}, // Yellow
			{450.f, 550.f}, // Position, extreme left of screen between the top and middle
			{1.f, -0.5f}, // Heading
			{60.f}, // Radius
			{256.f} // Size of the beam
		);

		float speed = 0.1f;
		float force = 0.002f;

		vec2 velocity = {0.f, 0.f};
		vec2 acceleration = {0.f, 0.f};

		bool test_sensor_activity(const Sensor& s, const Obstacle& obstacle) {
			return (maths::point_segment_intersect(obstacle.position, s.start, s.position, s.end, s.radius));
		}

		void move(const Obstacle& obstacle) {
			if (test_sensor_activity(left_sensor, obstacle)) {

			}

			if (test_sensor_activity(right_sensor, obstacle)) {

			}
		}
	};

	
}

namespace simulation {
	void init();
	void update();
	void draw();
	void destroy();

	extern Vehicle vehicle;

	extern Obstacle obstacle;

	extern GLuint sensor_VAO;
	extern GLuint sensor_UBO;
	extern Shader sensor_shader;

	extern GLuint obstacle_VAO;
	extern GLuint obstacle_UBO;
	extern Shader obstacle_shader;

	extern vec2 resolution;
	extern vec2 near_far;
	extern mat4 resolution_matrix;

	extern vec2 cursor_position;
}

