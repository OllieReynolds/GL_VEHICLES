#pragma once

#include <glew.h>
#include <glfw3.h>

#include "boundary.h"
#include "cube.h"
#include "draw_line.h"
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
		vec2 cursor_position;
		vec2 resolution;
		vec2 near_far;
		//mat4 resolution_matrix;
	
		// Simulation components
		Boundary boundary;
		Obstacle obstacle;
		std::vector<Vehicle*> vehicles;

		// GUI components
		Text text;
		int state;
		int num_vehicles;

		// Misc.
		bool draw_sensors;
		//Cube cube;
		std::vector<Cube*> cubes;

		Draw_Line line;

		int selection_vehicle;


		mat4 view_matrix;
		mat4 perspective_matrix;

		float fov;
	};
}

