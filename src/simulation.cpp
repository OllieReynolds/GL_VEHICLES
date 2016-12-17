#include "..\include\simulation.h"

namespace simulation {
	void Simulation::init() {
		state               = 1;
		selection_vehicle   = 0;
		num_vehicles        = 50;
		fov                 = 90.f;
		draw_sensors        = false;
		cursor_position     = vec2{   0.f,   0.f};
		resolution          = vec2{1366.f, 768.f};
		near_far            = vec2{  -1.f,   1.f};
		observer_position   = vec3{   0.f, 100.f, 0.f};
		view_matrix         = shared::view_matrix(observer_position, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
		perspective_matrix  = shared::perspective_matrix(fov, 1.7786f, 0.1f, 5000.f);
		orthographic_matrix = maths::orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());
		boundary            = Boundary{Transform{vec3{resolution.x * 0.665f, resolution.y * 0.73f, 0.f}, vec3{340.f, 200.f, 0.f}, 0.f}, vec4{1.f, 1.f, 1.f, 0.05f}};
		text                = Text{24, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
		line                = new Draw_Line();
		quad_renderer       = DrawQuad();

		boundary.init(orthographic_matrix);
		text.init_text(resolution);
		line->init_line(perspective_matrix);
		quad_renderer.init_quad(orthographic_matrix);

		for (int i = 0; i < num_vehicles; i++) {
			float random_x = utils::gen_random(-80.f, 80.f);
			float random_z = utils::gen_random(-80.f, 80.f);
			vec3  position = vec3{random_x, 0.f, random_z};
			vec3      size = vec3{4.f, 2.f, 2.f};
			float rotation = utils::gen_random(0.f, 360.f);
			Transform    t = Transform{position, size, rotation};

			vec4    colour = vec4{1.f};
			float    speed = utils::gen_random(0.2f, 0.5f);
			float steering = utils::gen_random(3.5f, 4.5f);

			Vehicle*     v = new Vehicle{t, colour, steering, speed};
			
			v->init(perspective_matrix);
			vehicles.push_back(v);
		}
	}

	void Simulation::update() {
		perspective_matrix = shared::perspective_matrix(fov, 1.7786f, 0.1f, 1000.f);
		view_matrix = shared::view_matrix(observer_position, vehicles.at(selection_vehicle)->position, {0.f, 1.f, 0.f});

		if (state == 1) {
			boundary.update(cursor_position);

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
			vec3 line_end = vec3{v->position.x + scaled_direction.x, 0.f, v->position.z + scaled_direction.y};

			line->colour = (v->detected) ? vec4{0.f, 1.f, 0.f, 1.f} : vec4{1.f, 1.f, 0.f, 1.f};
			line->draw_line(view_matrix, perspective_matrix, line_end, v->position);
		}

		line->colour = {1.f, 0.f, 0.f, 1.f};
		line->draw_line(view_matrix, perspective_matrix, vehicles.at(selection_vehicle)->position, {0.f, 50.f, 0.f});
		glDisable(GL_DEPTH_TEST);

		
		boundary.draw(view_matrix, perspective_matrix);

		quad_renderer.draw_quad(view_matrix, perspective_matrix, {00.f, 0.f}, {340.f, 200.f});

		float rel = 0.8f;
		for (std::string s : vehicles.at(selection_vehicle)->string_attribs()) {
			text.draw_text(s, vec2{1366.f * 0.55f, 768.f * rel}, false);
			rel -= 0.05f;
		}
	}

	void Simulation::destroy() {
		line->destroy_line();
		delete line;

		quad_renderer.destroy_quad();

		text.destroy_text();

		for (Vehicle* v : vehicles) {
			v->destroy();
			delete v;
		}
	}
}