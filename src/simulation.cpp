#include "..\include\simulation.h"

namespace simulation {
	void Simulation::init() {
		draw_sensors      = false;
		cursor_position   = vec2{0.f, 0.f};
		resolution        = vec2{1366.f, 768.f};
		near_far          = vec2{-1.f, 1.f};
		resolution_matrix = orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());

		Transform boundary_transform = Transform{vec3{resolution.x * 0.5f, resolution.y * 0.6f, 0.f}, vec3{resolution.x * 0.95f, resolution.y * 0.75f, 0.f}, 0.f};
		boundary = Boundary{boundary_transform, vec4{1.f, 1.f, 1.f, 0.05f}};

		Transform obstacle_transform = Transform{vec3{600.f, 450, 0.f}, vec3{16.f, 16.f, 0.f}, 0.f};
		obstacle = Obstacle{obstacle_transform, vec4{1.f, 1.f, 1.f, 0.7f}};

		for (int i = 0; i < 100; i++) {
			vehicles.push_back(new Vehicle{
				Transform{
					vec3{utils::gen_random(100.f, 1200.f), utils::gen_random(200.f, 700.f), 0.f},
					vec3{20.f, 10.f, 0.f},
					utils::gen_random(0.f, 360.f)
				},
				vec4{1.f, 1.f, 1.f, 1.f}, 
				utils::gen_random(2.5f, 3.5f),
				utils::gen_random(1.5f, 2.5f)
			});
		}

		text = Text{24, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
	
		

		boundary.init();
		obstacle.init();

		for (Vehicle* v : vehicles)
			v->init();

		text.init_text(resolution);

		for (int x = 0; x < 8; x++) {
			cubes.push_back(new Cube{
				Transform{vec3{utils::gen_random(0.f, 8.f), 0.f, utils::gen_random(0.f, 8.f)},
				vec3{1.f},
				utils::gen_random(0.f, 360.f)
			},
				vec4{1.f, 1.f, 1.f, 1.f},
			});
		}

		for (Cube* c : cubes)
			c->init();

		state = 1;
	}

	void Simulation::update() {
		if (state == 1) {
			boundary.update(cursor_position);
			obstacle.update(cursor_position);

			std::vector<vec2> locations;
			for (Vehicle* v : vehicles) {
				locations.push_back(v->position.XY());
			}

			for (Vehicle* v : vehicles) {
				v->other_vehicle_locations = locations;
				v->update(cursor_position);
			}
		}

		for (Cube* c : cubes)
			c->update(cursor_position);
	}

	void Simulation::draw() {
		glEnable(GL_DEPTH_TEST);
		for (Cube* c : cubes)
			c->draw();
		glDisable(GL_DEPTH_TEST);


		glEnable(GL_BLEND);
		for (Vehicle* v : vehicles) {
			v->draw();
			if (draw_sensors) v->draw_sensors();
		}

		boundary.draw();
		obstacle.draw();

		text.draw_text("  CURSOR: Obstacle", vec2(1366.f * 0.5f, 128.f), true);
		text.draw_text(" VEHICLE: Excitory", vec2{1366.f * 0.5f, 64.f}, true);

		int cX = cursor_position.x;
		int cY = cursor_position.y;
		text.draw_text(std::to_string(cX) + "  " + std::to_string(cY), vec2{cursor_position.x, cursor_position.y}, true);
		glDisable(GL_BLEND);
	}

	void Simulation::destroy() {
		for (Cube* c : cubes) {
			c->destroy();
			delete c;
		}


		obstacle.destroy();
		

		for (Vehicle* v : vehicles) {
			v->destroy();
			delete v;
		}

		text.destroy_text();
	}
}