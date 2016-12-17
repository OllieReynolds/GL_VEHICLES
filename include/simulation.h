#pragma once

#include <glew.h>
#include <glfw3.h>

#include "boundary.h"
#include "draw_line.h"
#include "draw_quad.h"
#include "maths.h"
#include "obstacle.h"
#include "text.h"
#include "vehicle.h"

using namespace maths;
using namespace utils;

namespace simulation {
	class Simulation {
	public:
		void init();
		void update();
		void draw();
		void destroy();
	
		// General Settings
		bool draw_sensors;
		int selection_vehicle;
		float fov;

		vec2 cursor_position;
		vec2 resolution;
		vec2 near_far;
		vec3 observer_position;

		mat4 view_matrix;
		mat4 perspective_matrix;
		mat4 orthographic_matrix;

		// Simulation components
		Boundary boundary;
		Draw_Line* line;
		DrawQuad quad_renderer;
		std::vector<Vehicle*> vehicles;
		
		// GUI components
		Text text;
		int state;
		int num_vehicles;
	};
}

