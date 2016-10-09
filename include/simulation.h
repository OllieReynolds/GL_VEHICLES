#pragma once

#include <glew.h>
#include <glfw3.h>

#include "maths.h"
#include "obstacle.h"
#include "text.h"
#include "vehicle.h"

using namespace maths;
using namespace utils;

namespace simulation {
	void init();
	void update();
	void draw();
	void destroy();
	
	// General Settings
	extern vec2 cursor_position;
	extern vec2 resolution;
	extern vec2 near_far;
	extern mat4 resolution_matrix;
	
	// Simulation components
	extern Obstacle obstacle;
	extern Vehicle vehicle;

	// GUI components
	extern Text text;
}

