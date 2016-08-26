#pragma once

#include <vector>

#include <glew.h>
#include <glfw3.h>

#include "maths.h"
#include "polygon.h"

namespace utils {
	class Simulation {
	public:
		Simulation(int num_beams) {
			for (int i = 0; i < num_beams; ++i) {
				segments.push_back(
				    maths::segment{}
				);
			}
		}

		void init_simulation();
		void draw_simulation(const float fps);
		void destroy_simulation();

		Polygon polygon;

		std::vector<maths::segment> segments;
	};
}