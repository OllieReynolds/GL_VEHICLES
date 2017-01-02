#pragma once

#include <glew.h>
#include <glfw3.h>

#include "cube_renderer.h"
#include "line_renderer.h"
#include "quad_renderer.h"
#include "text_renderer.h"
#include "mesh_renderer.h"
#include "circle_renderer.h"
#include "maths.h"
#include "model.h"

using namespace maths;
using namespace utils;

namespace simulation {
	struct Vehicle_Attributes {
		float forward_speed;
		float turning_speed;
	};

	struct Button_Attributes {
		vec2 position;
		vec2 size;
		vec4 colour;
		std::string label;
	};

	struct Wheel_Attributes {
		float angular_offset;
		float y_rotation;

		Transform gen_transform_from_vehicle(const Transform& t, float wheel_dist) {
			float wheel_offset_from_vehicle_angle = t.rotation.y - angular_offset;
			vec2 direction = polar_to_cartesian(to_radians(wheel_offset_from_vehicle_angle)) * wheel_dist;

			Transform transform;
			transform.position = t.position + vec3{ direction.x, 2.f, direction.y };
			transform.size = vec3{ 1.f, 1.f, 1.f };
			transform.rotation = t.rotation;
			transform.rotation.y += y_rotation;

			if (y_rotation == 0.f)
				transform.rotation.z += 128.f * utils::elapsed_time();
			else 
				transform.rotation.z -= 128.f * utils::elapsed_time();

			return transform;
		}
	};



	class Simulation {
	public:
		Simulation();

		void init();
		void update();
		void draw();
		void destroy();

		Cube_Renderer cube_renderer;
		Line_Renderer line_renderer;
		Quad_Renderer quad_renderer;
		Text_Renderer text_renderer;
		Mesh_Renderer mesh_renderer;
		Circle_Renderer circle_renderer;

		Model model;

		Texture wheel_texture;
		Texture floor_texture;

		bool draw_sensors;
		bool follow_vehicle;
		bool mouse_pressed;
		
		int index_selected_vehicle;
		int index_state;
		int index_active_button;
		int index_pressed_button;

		int num_vehicles;

		int num_buttons;
		
		float follow_cam_distance;
		float fov;
		float aspect;

		vec2 cursor_position;
		vec2 resolution;
		vec2 near_far_ortho;
		vec2 near_far_persp;
		
		vec3 start_cam_position;
		vec3 cam_position;
		vec3 cam_target;
		vec3 up;
		
		mat4 view_matrix;
		mat4 perspective_matrix;
		mat4 orthographic_matrix;

		Button_Attributes* button_attributes;
		Vehicle_Attributes* vehicle_attributes;
		Wheel_Attributes* wheel_attributes;

		utils::Transform* vehicle_transforms;


	private:
		void update_vehicle(utils::Transform& transform, Vehicle_Attributes attribs);
		void update_camera();
		void update_ui();


		void draw_vehicles();
		void draw_ui();
		void draw_environment();
	};
}

