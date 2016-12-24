#include "..\include\simulation.h"

namespace simulation {
	void Simulation::init() {
		state               = 1;
		selection_vehicle   = 0;
		num_vehicles        = 50;
		fov                 = 90.f;
		follow_cam_distance = 20.f;
		draw_sensors        = false;
		follow_vehicle      = false;
		mouse_pressed       = false;
		cursor_position     = vec2{0.f, 0.f};
		resolution          = vec2{1366.f, 768.f};
		near_far_ortho      = vec2{-1.f, 1.f};
		near_far_persp      = vec2{0.1f, 1000.f};
		cam_position        = vec3{0.f, 100.f, 0.f};
		cam_target          = vec3{0.f, 0.f, 0.f };
		up                  = vec3{0.f, 1.f, 0.f };
		aspect              = resolution.x / resolution.y;
		view_matrix         = shared::view_matrix(cam_position, cam_target, up);
		perspective_matrix  = shared::perspective_matrix(fov, aspect, near_far_persp.x, near_far_persp.y);
		orthographic_matrix = maths::orthographic_matrix(resolution, near_far_ortho.x, near_far_ortho.y, maths::mat4());
		
		cube_renderer		= Cube_Renderer();
		line_renderer		= Line_Renderer();
		quad_renderer		= Quad_Renderer();
		hud                 = Hud();

		cube_renderer.init();
		line_renderer.init();
		quad_renderer.init("C:/Users/Ollie/Desktop/debug.png");
		hud.init();

		for (int i = 0; i < num_vehicles; i++)
			add_vehicle();
	}

	void Simulation::update() {
		if (state == 1)
			update_vehicles();

		update_ui();

		hud.update(cursor_position, mouse_pressed);
		mouse_pressed = false;
	}

	void Simulation::update_vehicles() {
		std::vector<vec2> locations;
		for (Vehicle* v : vehicles) {
			locations.push_back(v->position.XZ());
		}

		for (Vehicle* v : vehicles) {
			v->other_vehicle_locations = locations;
			v->update(cursor_position);
		}
	}

	void Simulation::update_ui() {
		if (follow_vehicle) {
			vec2 direction = vehicles.at(selection_vehicle)->direction;
			cam_target = cam_position + vec3{ direction.x, 0.f, direction.y };

			direction *= follow_cam_distance;

			cam_position = vehicles.at(selection_vehicle)->position;
			cam_position.x -= direction.x;
			cam_position.y += follow_cam_distance;
			cam_position.z -= direction.y;
		}
		else {
			cam_position = vec3{ 0.f, 100.f, 0.f };
			cam_target = vehicles.at(selection_vehicle)->position;
		}

		perspective_matrix = shared::perspective_matrix(fov, aspect, near_far_persp.x, near_far_persp.y);
		view_matrix = shared::view_matrix(cam_position, cam_target, up);
	}

	void Simulation::draw() {
		glEnable(GL_DEPTH_TEST);
		draw_vehicles();
		draw_floor();
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		draw_ui();
		glDisable(GL_BLEND);
	}

	void Simulation::destroy() {
		cube_renderer.destroy();
		line_renderer.destroy();
		quad_renderer.destroy();
		hud.destroy();

		for (Vehicle* v : vehicles) {
			v->destroy();
			delete v;
		}
	}


	void Simulation::add_vehicle() {
		vec3  position = vec3{ 0.f, 4.f, 0.f };
		vec3      size = vec3{ 8.f, 4.f, 4.f };
		float rotation = utils::gen_random(0.f, 360.f);
		Transform    t = Transform{ position, size, rotation };

		vec4    colour = vec4{ 1.f, 0.f, 1.f, 0.f };
		float    speed = utils::gen_random(0.2f, 0.5f);
		float steering = utils::gen_random(3.5f, 4.5f);

		Vehicle*     v = new Vehicle{ t, colour, steering, speed };

		v->init(perspective_matrix);
		vehicles.push_back(v);
	}

	void Simulation::remove_vehicle() {
		vehicles.back()->destroy();
		delete vehicles.back();
	}

	void Simulation::edit_vehicle() {

	}

	void Simulation::play() {
		state = 1;
	}

	void Simulation::pause() {
		state = -1;
	}

	void Simulation::draw_floor() {
		quad_renderer.draw_3D(view_matrix, perspective_matrix, { 0.f, 0.f, 0.f }, { 400.f }, { 90.f, 0.f, 0.f }, utils::data::colour::dark_grey);
	}

	void Simulation::draw_vehicles() {

		

		//cube_renderer.draw_multiple(num_vehicles, view_matrix, perspective_matrix, )

		for (Vehicle* v : vehicles) {
			cube_renderer.draw(view_matrix, perspective_matrix, v->position, v->size, v->rotation, v->colour);

			vec2 scaled_direction = v->direction * 20.f;
			vec3 line_end = vec3{ v->position.x + scaled_direction.x, 0.f, v->position.z + scaled_direction.y };

			line_renderer.colour = (v->detected) ? data::colour::green : data::colour::red;
			line_renderer.draw(view_matrix, perspective_matrix, line_end, v->position);
		}
	}

	void Simulation::draw_ui() {
		hud.draw(orthographic_matrix, view_matrix);
	}
}