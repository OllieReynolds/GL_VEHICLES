#include "..\include\simulation.h"

namespace simulation {
	void Simulation::init() {
		state               = 1;
		selection_vehicle   = 0;
		num_vehicles        = 50;
		fov                 = 90.f;
		draw_sensors        = false;
		follow_vehicle      = false;
		cursor_position     = vec2{0.f, 0.f};
		resolution          = vec2{1366.f, 768.f};
		near_far            = vec2{-1.f, 1.f};
		observer_position   = vec3{0.f, 100.f, 0.f};
		view_matrix         = shared::view_matrix(observer_position, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
		perspective_matrix  = shared::perspective_matrix(fov, 1.7786f, 0.1f, 5000.f);
		orthographic_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
		
		line                = Line_Renderer();
		quad_renderer       = Quad_Renderer();
		hud                 = Hud();

		line.init_line(perspective_matrix);
		quad_renderer.init_quad(orthographic_matrix);
		hud.init(orthographic_matrix);

		for (int i = 0; i < num_vehicles; i++) {
			float random_x = utils::gen_random(-80.f, 80.f);
			float random_z = utils::gen_random(-80.f, 80.f);
			vec3  position = vec3{random_x, 4.f, random_z};
			vec3      size = vec3{8.f, 4.f, 4.f};
			float rotation = utils::gen_random(0.f, 360.f);
			Transform    t = Transform{position, size, rotation};

			vec4    colour = vec4{1.f, 0.f, 1.f, 0.f};
			float    speed = utils::gen_random(0.2f, 0.5f);
			float steering = utils::gen_random(3.5f, 4.5f);

			Vehicle*     v = new Vehicle{t, colour, steering, speed};
			
			v->init(perspective_matrix);
			vehicles.push_back(v);
		}
	}

	void Simulation::update() {
		
		if (state == 1) {
			std::vector<vec2> locations;
			for (Vehicle* v : vehicles) {
				locations.push_back(v->position.XZ());
			}

			for (Vehicle* v : vehicles) {
				v->other_vehicle_locations = locations;
				v->update(cursor_position);
			}
		}

		if (follow_vehicle) {
			observer_position = vehicles.at(selection_vehicle)->position;
			observer_position.y += 10.f;


			vec2 direction = vehicles.at(selection_vehicle)->direction;
			vec3 target = observer_position + vec3{direction.x, 0.f, direction.y};

			float dist = 20.f;
			observer_position.x -= (direction.x * dist);
			observer_position.z -= (direction.y * dist);

			perspective_matrix = shared::perspective_matrix(fov, 1.7786f, 0.1f, 1000.f);
			view_matrix = shared::view_matrix(observer_position, target, {0.f, 1.f, 0.f});
		}
		else {
			observer_position = vec3{0.f, 100.f, 0.f};
			vec3 target = vehicles.at(selection_vehicle)->position;
			perspective_matrix = shared::perspective_matrix(fov, 1.7786f, 0.1f, 1000.f);
			view_matrix = shared::view_matrix(observer_position, target, {0.f, 1.f, 0.f});
		}
	}

	void Simulation::draw() {
		glEnable(GL_DEPTH_TEST);
		for (Vehicle* v : vehicles) {
			v->draw(view_matrix, perspective_matrix);
			
			vec2 scaled_direction = v->direction * 20.f;
			vec3 line_end = vec3{v->position.x + scaled_direction.x, 0.f, v->position.z + scaled_direction.y};

			line.colour = (v->detected) ? vec4{0.f, 1.f, 0.f, 1.f} : vec4{1.f, 1.f, 0.f, 1.f};
			line.draw_line(view_matrix, perspective_matrix, line_end, v->position);
		}
		quad_renderer.draw_quad_3D(view_matrix, perspective_matrix, {0.f, 0.f, 0.f}, {400.f}, {90.f, 0.f, 0.f}, utils::colour::dark_grey);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		line.draw_line(view_matrix, perspective_matrix, vehicles.at(selection_vehicle)->position, {0.f, 50.f, 0.f});

		//float inc = 1366.f / 6.f;

		hud.draw(orthographic_matrix, view_matrix);

		/*for (int i = 0; i < 6; i++) {
			float x = (inc * 0.5f) + (i * inc);
			float y = 24.f;

			quad_renderer.draw_quad_2D(view_matrix, orthographic_matrix, {x, y}, {1366.f / 6.2f, 64.f * 0.6f}, {0.1f, 0.1f, 0.1f, 1.f});
			text.draw_text("test", {x, y}, true);
		}*/

		/*float rel = 0.8f;
		for (std::string s : vehicles.at(selection_vehicle)->string_attribs()) {
			text.draw_text(s, vec2{1366.f * 0.55f, 768.f * rel}, false);
			rel -= 0.05f;
		}*/
		glDisable(GL_BLEND);
	}

	void Simulation::destroy() {
		line.destroy_line();

		quad_renderer.destroy_quad();


		hud.destroy();

		for (Vehicle* v : vehicles) {
			v->destroy();
			delete v;
		}
	}
}