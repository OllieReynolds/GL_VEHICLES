#include "..\include\simulation.h"

namespace simulation {
	Simulation::Simulation() {
		
	}

	void Simulation::init() {
		state				= 1;
		selection_vehicle	= 0;
		num_vehicles		= 50;
		active_button       = -1;
		pressed_button		= -1;
		num_buttons         = 6;
		fov					= 90.f;
		follow_cam_distance = 20.f;
		draw_sensors		= false;
		follow_vehicle		= false;
		mouse_pressed		= false;
		cursor_position		= vec2{ 0.f, 0.f };
		resolution			= vec2{ 1366.f, 768.f };
		near_far_ortho		= vec2{ -1.f, 1.f };
		near_far_persp		= vec2{ 0.1f, 1000.f };
		cam_position		= vec3{ 0.f, 0.f, 0.f };
		start_cam_position  = vec3{ 0.f, 100.f, 0.f };
		cam_target			= vec3{ 0.f, 0.f, 0.f };
		up					= vec3{ 0.f, 1.f, 0.f };
		aspect				= resolution.x / resolution.y;
		view_matrix			= shared::view_matrix(cam_position, cam_target, up);
		perspective_matrix	= shared::perspective_matrix(fov, aspect, near_far_persp.x, near_far_persp.y);
		orthographic_matrix = maths::orthographic_matrix(resolution, near_far_ortho.x, near_far_ortho.y, maths::mat4());
		cube_renderer		= Cube_Renderer();
		line_renderer		= Line_Renderer();
		quad_renderer		= Quad_Renderer();
		text_renderer		= Text_Renderer(18, "data/ShareTechMono-Regular.ttf");
		mesh_renderer		= Mesh_Renderer();
		circle_renderer		= Circle_Renderer();
		mesh                = Mesh();
		

		vehicle_transforms = new utils::Transform[num_vehicles];
		vehicle_attributes = new Vehicle_Attributes[num_vehicles];
		for (int i = 0; i < num_vehicles; i++) {
			vehicle_transforms[i] = {
				vec3{ 0.f, 2.1f, 0.f },
				vec3{ 8.f, 4.f, 4.f },
				vec3{ 0.f, utils::gen_random(0.f, 360.f), 0.f}
			};

			vehicle_attributes[i] = {
				utils::gen_random(0.2f, 0.5f),
				utils::gen_random(2.5f, 3.5f)
			};
		}

		button_attributes = new Button_Attributes[num_buttons];
		button_attributes[0] = { { 100.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "add vehicle" };
		button_attributes[1] = { { 300.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "remove vehicle" };
		button_attributes[2] = { { 500.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "follow vehicle" };
		button_attributes[3] = { { 700.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "pause" };
		button_attributes[4] = { { 900.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "play" };
		button_attributes[5] = {{ 1100.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "edit vehicle" };

		wheel_texture.init("C:/Users/Ollie/Desktop/wheel_texture.png", 1024, 1024);
		floor_texture.init("C:/Users/Ollie/Desktop/debug.png", 1024, 1024);

		mesh.init("C:/Users/Ollie/Desktop/vehicle/wheel_tex.obj");

		cube_renderer.init();
		line_renderer.init();
		quad_renderer.init();
		text_renderer.init(resolution);
		circle_renderer.init();
		mesh_renderer.init(mesh);
	}

	void Simulation::update() {
		for (int i = 0; i < num_vehicles; i++)
			update_vehicle(vehicle_transforms[i], vehicle_attributes[i]);
		
		update_ui();
		update_camera();

		mouse_pressed = false;
	}

	void Simulation::update_vehicle(utils::Transform& transform, Vehicle_Attributes attribs) {
		if (!point_quad_intersect(transform.position.XZ(), -100, 100, 100, -100))
			transform.rotation.y += attribs.turning_speed;

		vec2 direction = polar_to_cartesian(to_radians(transform.rotation.y));
		vec2 velocity = direction * attribs.forward_speed;
		transform.position += {velocity.x, 0.f, velocity.y};
	}

	void Simulation::update_camera() {
		if (follow_vehicle) {
			vec2 direction = polar_to_cartesian(to_radians(vehicle_transforms[selection_vehicle].rotation.y));
			direction *= follow_cam_distance;

			cam_position = vehicle_transforms[selection_vehicle].position;
			cam_position.x -= direction.x;
			cam_position.y += follow_cam_distance;
			cam_position.z -= direction.y;
			cam_target = cam_position + vec3{ direction.x, -20.f, direction.y };
		}
		else {
			cam_position = start_cam_position;
			cam_target = vehicle_transforms[selection_vehicle].position;
		}

		perspective_matrix = shared::perspective_matrix(fov, aspect, near_far_persp.x, near_far_persp.y);
		view_matrix = shared::view_matrix(cam_position, cam_target, up);
	}

	void Simulation::update_ui() {
		active_button = -1;
		pressed_button = -1;
		for (int i = 0; i < num_buttons; i++) {
			float l = button_attributes[i].position.x - (button_attributes[i].size.x * 0.5f);
			float r = button_attributes[i].position.x + (button_attributes[i].size.x * 0.5f);
			float u = button_attributes[i].position.y + (button_attributes[i].size.y * 0.5f);
			float d = button_attributes[i].position.y - (button_attributes[i].size.y * 0.5f);
			if (utils::point_quad_intersect(cursor_position, l, r, u, d))
				active_button = i;
		}

		if (mouse_pressed && active_button != -1)
			pressed_button = active_button;
	}

	void Simulation::draw() {
		glEnable(GL_DEPTH_TEST);
		cube_renderer.draw_multiple(num_vehicles, view_matrix, perspective_matrix, vehicle_transforms, utils::data::colour::blue);
		quad_renderer.draw_3D_textured(view_matrix, perspective_matrix, { 0.f, 0.f, 0.f }, { 400.f }, { 90.f, 0.f, 0.f }, floor_texture);
		circle_renderer.draw_3D(view_matrix, perspective_matrix, vehicle_transforms[selection_vehicle].position - vec3{ 0.f, 2.f, 0.f }, { 12.f }, { 90.f, 0.f, 0.f }, utils::data::colour::yellow, false);
		mesh_renderer.draw_3D_textured(mesh, view_matrix, perspective_matrix, { { 20.f, 10.f, 100.f }, { 10.f }, { 0.f, 180.f, utils::elapsed_time() * 32.f } }, wheel_texture);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		draw_ui();
		glDisable(GL_BLEND);
	}

	void Simulation::draw_ui() {
		for (int i = 0; i < num_buttons; i++) {
			quad_renderer.draw_2D(view_matrix, orthographic_matrix, button_attributes[i].position, button_attributes[i].size, button_attributes[i].colour);
			text_renderer.draw(button_attributes[i].label, button_attributes[i].position, true, data::colour::white);
		}

		if (active_button != -1) {
			text_renderer.draw("wow", button_attributes[active_button].position, true, data::colour::white);
		}

		if (pressed_button != -1) {
			vec2 position = { button_attributes[active_button].position.x, button_attributes[active_button].position.y - 50 };
			text_renderer.draw("go", position, true, data::colour::white);
		}
	}

	void Simulation::destroy() {
		cube_renderer.destroy();
		line_renderer.destroy();
		quad_renderer.destroy();
		text_renderer.destroy();
		circle_renderer.destroy();
		mesh_renderer.destroy();

		wheel_texture.destroy();
		floor_texture.destroy();

		delete[] vehicle_transforms;
		delete[] vehicle_attributes;
		delete[] button_attributes;
	}
}