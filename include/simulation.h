#pragma once

#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "maths.h"
#include "obstacle.h"
#include "sensor.h"
#include "shader.h"
#include "utils.h"
#include "vehicle.h"

using namespace maths;
using namespace utils;

namespace simulation {
	void init();
	void update();
	void draw();
	void destroy();

	extern Vehicle vehicle;
	extern Obstacle obstacle;
	



	extern vec2 resolution;
	extern vec2 near_far;
	extern mat4 resolution_matrix;

	extern vec2 cursor_position;
}

