#pragma once

#include <glew.h>
#include <glfw3.h>

#include "hud.h"
#include "line_renderer.h"
#include "quad_renderer.h"
#include "maths.h"
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

		Line_Renderer line;
		Quad_Renderer quad_renderer;

		Hud hud;

		std::vector<Vehicle*> vehicles;		

		vec2 cursor_position;		
		vec3 observer_position;

		int state;
		int num_vehicles;
		bool draw_sensors;
		bool follow_vehicle;
		int selection_vehicle;

		float fov;
		vec2 resolution;
		vec2 near_far;
		mat4 view_matrix;
		mat4 perspective_matrix;
		mat4 orthographic_matrix;
	};
}

