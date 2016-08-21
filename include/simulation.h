#pragma once

#include <glew.h>
#include <glfw3.h>

#include "polygon.h"

namespace utils {
	class Simulation {
	public:
		Simulation() {}

		void init_simulation();
		void draw_simulation(const float fps);
		void destroy_simulation();

		Polygon polygon;
	};
}