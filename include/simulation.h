#pragma once

#include <glew.h>
#include <glfw3.h>

#include "polygon.h"

namespace utils {
	class Simulation {
	public:
		Simulation();

		void init_simulation();
		void update_simulation(GLFWwindow* window);
		void draw_simulation(const float fps);
		void destroy_simulation();


	private:
		Polygon p;
	};
}