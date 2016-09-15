#pragma once

#include <cmath>
#include <complex>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "maths.h"
#include "shader.h"
#include "utils.h"

using namespace maths;
using namespace utils;

namespace simulation {
	void init();
	void update();
	void draw();
	void destroy();

	static const vec2 resolution = {1366.f, 768.f};
	static const vec2 near_far = {-1.f, 1.f};
	static const mat4 resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
}

namespace logic {
	struct Sector {
		vec4 colour;
		vec2 position;
		vec2 start;
		vec2 end;
		float radius;
	};

	struct Obstacle {
		vec4 colour;
		vec2 position;
		vec2 size;
	};

	

	static Sector create_sector(const vec4& colour, const vec2& position, const vec2& heading, const float theta, const float radius) {
		float heading_angle = atan2(heading.y, heading.x) * 180 / std::_Pi;

		float start_arm_angle_deg = heading_angle - theta * 0.5f;
		float end_arm_angle_deg = heading_angle + theta * 0.5f;

		float start_arm_angle_rad = start_arm_angle_deg * std::_Pi / 180;
		float end_arm_angle_rad = end_arm_angle_deg * std::_Pi / 180;

		vec2 start = {cos(start_arm_angle_rad), sin(start_arm_angle_rad)};
		vec2 end = {cos(end_arm_angle_rad), sin(end_arm_angle_rad)};

		return{colour, position, start, end, radius};
	}


	void move_vehicle();
	

// ######################################################

	

	static Sector sensor = create_sector(
		{1.f, 1.f, 0.f, 1.f}, // Yellow
		{40.f, 600.f}, // Position, extreme left of screen between the top and middle
		{1.f, 0.f}, // Heading
		{60.f}, // Radius
		{602.f} // Size of the beam
	);

	static Obstacle obstacle = {
		{1.f, 1.f, 1.f, 1.f}, // White
		{600.f, 450}, // In line with the sensor for now
		{32.f} // Smallish
	};
}

namespace graphics {
	static GLuint sensor_VAO;
	static GLuint sensor_UBO;
	static Shader sensor_shader;

	static GLuint obstacle_VAO;
	static GLuint obstacle_UBO;
	static Shader obstacle_shader;
}