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
	void draw(const float fps);
	void destroy();

	static const vec2 resolution = {1366.f, 768.f};
	static const vec2 near_far = {-1.f, 1.f};
	static const mat4 resolution_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
}

namespace logic {
	struct Sector {
		vec4 colour;
		vec2 position;
		vec2 size;
		float rotation;
		float theta;
		bool is_body;
	};

	void move_vehicle();

	static float speed = 2.f;

	static vec2 acceleration = {0.f, 0.f};
	static vec2 velocity = {0.f, 0.f};

	static Sector light_FOV_L = {
		{1.f, 1.f, 0.f, 1.f},
		{-10.f, 510.f},
		{128.f},
		0.f,
		40.f,
		false
	};

	static Sector light_FOV_R = {
		{1.f, 1.f, 0.f, 1.f},
		{-10.f, 546.f},
		{128.f},
		0.f,
		40.f,
		false
	};

	static Sector body = {
		{1.f, 1.f, 1.f, 1.f},
		{-64.f, 528.f},
		{64.f, 32.f},
		0.f,
		40.f,
		true
	};

	static Sector box = {
		{1.f, 0.f, 0.f, 1.f},
		{593.f, 510.f},
		{8.f},
		0.f,
		40.f,
		true
	};
}

namespace graphics {
	static GLuint VAO;
	static GLuint UBO;

	static Shader shader;
}