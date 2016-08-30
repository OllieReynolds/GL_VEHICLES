#pragma once

#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "maths.h"
#include "shader.h"
#include "utils.h"

namespace simulation {
	class Simulation {
	public:
		Simulation() {}

		void init_simulation();
		void update_simulation();
		void draw_simulation(const float fps);
		void destroy_simulation();
	};
}