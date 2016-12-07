#include "..\include\simulation.h"

namespace simulation {
	void Simulation::init() {
		state             = 1;
		selection_vehicle = 0;
		num_vehicles      = 50;
		fov               = 90.f;
		draw_sensors      = false;
		cursor_position   = vec2{0.f, 0.f};
		resolution        = vec2{1366.f, 768.f};
		near_far          = vec2{-1.f, 1.f};

		vec3 still_pos = {0.f, 30.f, 150.f};
		view_matrix = shared::view_matrix(
			still_pos,
			{0.f, 0.f, 0.f},
			{0.f, 1.f, 0.f}
		);

		perspective_matrix = shared::perspective_matrix(fov, 1.7786f, 0.1f, 5000.f);

		//////////////////////////////
		// 2D
		//////////////////////////////
		Transform boundary_transform = Transform{
			vec3{resolution.x * 0.665f, resolution.y * 0.73f, 0.f}, 
			vec3{340.f, 200.f, 0.f}, 
			0.f
		};

		Transform obstacle_transform = Transform{
			vec3{600.f, 450, 0.f}, 
			vec3{16.f, 16.f, 0.f}, 
			0.f
		};

		boundary = Boundary{boundary_transform, vec4{1.f, 1.f, 1.f, 0.05f}};
		obstacle = Obstacle{obstacle_transform, vec4{1.f, 1.f, 1.f, 0.7f}};
		text     = Text{24, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};

		
		mat4 orthographic_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());

		boundary.init(orthographic_matrix);
		obstacle.init(orthographic_matrix);
		text.init_text(resolution);

		


		/////////////////////////////
		// 3D
		/////////////////////////////
		

		for (int i = 0; i < num_vehicles; i++) {
			vehicles.push_back(new Vehicle{
				Transform{
				vec3{utils::gen_random(-80.f, 80.f), 0.f, utils::gen_random(-80.f, 80.f)},
				vec3{4.f, 2.f, 2.f},
				utils::gen_random(0.f, 360.f)
			},
				vec4{1.f, 1.f, 1.f, 1.f},
				utils::gen_random(3.5f, 4.5f), // Turning Force
				utils::gen_random(0.25f, 0.55f)  // Speed
			});
		}

		line = Draw_Line();
		line.init_line(perspective_matrix);		

		for (Vehicle* v : vehicles)
			v->init(perspective_matrix);
	}

	void Simulation::update() {
		perspective_matrix = shared::perspective_matrix(fov, 1.7786f, 0.1f, 5000.f);

		vec3 still_pos = {0.f, 40.f, 170.f};
		view_matrix = shared::view_matrix(
			still_pos,
			vehicles.at(selection_vehicle)->position,
			{0.f, 1.f, 0.f}
		);

		if (state == 1) {
			boundary.update(cursor_position);
			obstacle.update(cursor_position);

			std::vector<vec2> locations;
			for (Vehicle* v : vehicles) {
				locations.push_back(v->position.XZ());
			}

			for (Vehicle* v : vehicles) {
				v->other_vehicle_locations = locations;
				v->update(cursor_position);
			}
		}
	}

	void Simulation::draw() {
		glEnable(GL_DEPTH_TEST);

		for (Vehicle* v : vehicles) {
			v->draw(view_matrix, perspective_matrix);
			vec2 scaled_direction = v->direction * 20.f;
			line.colour = (v->detected) ? vec4{0.f, 1.f, 0.f, 1.f} : vec4{1.f, 1.f, 0.f, 1.f};
			line.draw_line(view_matrix, perspective_matrix, {v->position.x + scaled_direction.x, 0.f, v->position.z + scaled_direction.y}, v->position);
			//v->draw_sensors();
		}

		line.colour = {1.f, 0.f, 0.f, 1.f};
		line.draw_line(view_matrix, perspective_matrix, vehicles.at(selection_vehicle)->position, {0.f, 50.f, 0.f});
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		boundary.draw(view_matrix, perspective_matrix);
		obstacle.draw(view_matrix, perspective_matrix);

		float rel = 0.8f;
		for (std::string s : vehicles.at(selection_vehicle)->string_attribs()) {
			text.draw_text(s, vec2{1366.f * 0.55f, 768.f * rel}, false);
			rel -= 0.05f;
		}

		int cX = cursor_position.x;
		int cY = cursor_position.y;
		text.draw_text(std::to_string(cX) + "  " + std::to_string(cY), vec2{cursor_position.x, cursor_position.y}, true);
	}

	void Simulation::destroy() {
		obstacle.destroy();
		line.destroy_line();
		text.destroy_text();

		for (Vehicle* v : vehicles) {
			v->destroy();
			delete v;
		}
	}
}