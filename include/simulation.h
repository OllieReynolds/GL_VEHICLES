#pragma once

#include <glew.h>
#include <glfw3.h>

#include "hud.h"
#include "cube_renderer.h"
#include "line_renderer.h"
#include "quad_renderer.h"
#include "text_renderer.h"
#include "maths.h"
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

		void add_vehicle();
		void remove_vehicle();
		void edit_vehicle();
		void play();
		void pause();

		Hud hud;
		Cube_Renderer cube_renderer;
		Line_Renderer line_renderer;
		Quad_Renderer quad_renderer;
		
		bool draw_sensors;
		bool follow_vehicle;
		bool mouse_pressed;
		
		int selection_vehicle;
		int state;
		int num_vehicles;
		
		float follow_cam_distance;
		float fov;
		float aspect;

		vec2 cursor_position;
		vec2 resolution;
		vec2 near_far_ortho;
		vec2 near_far_persp;
		
		vec3 cam_position;
		vec3 cam_target;
		vec3 up;
		
		mat4 view_matrix;
		mat4 perspective_matrix;
		mat4 orthographic_matrix;

		std::vector<Vehicle*> vehicles;

	private:
	

		void update_vehicles();
		void update_ui();

		void draw_floor();
		void draw_vehicles();
		void draw_ui();
	};
}

