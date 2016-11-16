#include "..\include\simulation.h"

namespace simulation {
	vec2 resolution;
	vec2 near_far;
	mat4 resolution_matrix;
	vec2 cursor_position;

	Boundary boundary;
	Obstacle obstacle;

	std::vector<Vehicle*> vehicles;

	Text text;
	int state;

	void init() {
		cursor_position   = vec2{0.f, 0.f};
		resolution        = vec2{1366.f, 768.f};
		near_far          = vec2{-1.f, 1.f};
		resolution_matrix = orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());

		Transform boundary_transform = Transform{vec2{resolution.x * 0.5f, resolution.y * 0.6f}, vec2{resolution.x * 0.95f, resolution.y * 0.75f}, 0.f};
		boundary = Boundary{boundary_transform, vec4{1.f, 1.f, 1.f, 0.05f}};

		Transform obstacle_transform = Transform{vec2{600.f, 450}, vec2{16.f, 16.f}, 0.f};
		obstacle = Obstacle{obstacle_transform, vec4{1.f, 1.f, 1.f, 0.7f}};

		for (int i = 0; i < 50; i++) {
			vehicles.push_back(new Vehicle{
				Transform{
					vec2{utils::gen_random(100.f, 1300.f), utils::gen_random(100.f, 600.f)},
					vec2{utils::gen_random(50.f, 100.f), 50.f}, 
					utils::gen_random(0.f, 360.f)
				},
				vec4{1.f, 1.f, 1.f, 1.f}, 
				0.002f
			});
		}

		//vehicles.push_back(new Vehicle{Transform{vec2{500.f, 550.f}, vec2{75.f, 50.f}, 115.f}, vec4{1.f, 1.f, 1.f, 1.f}, 0.002f});
		//vehicles.push_back(new Vehicle{Transform{vec2{800, 550.f}, vec2{75.f, 50.f}, 45.f}, vec4{1.f, 1.f, 1.f, 1.f}, 0.002f});

		text = Text{24, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
	
		boundary.init();
		obstacle.init();


		for (Vehicle* v : vehicles)
			v->init();

		text.init_text(resolution);

		state = 1;
	}

	void update() {
		

		if (state == 1) {
			boundary.update(cursor_position);
			obstacle.update(cursor_position);

			for (Vehicle* v : vehicles)
				v->update(cursor_position);
		}
	}

	void draw() {
		boundary.draw();
		obstacle.draw();

		for (Vehicle* v : vehicles)
			v->draw();

		text.draw_text("  CURSOR: Obstacle", vec2(1366.f * 0.5f, 128.f), true);
		text.draw_text(" VEHICLE: Excitory", vec2{1366.f * 0.5f, 64.f}, true);

		
		int cX = cursor_position.x;
		int cY = cursor_position.y;
		text.draw_text(std::to_string(cX) + "  " + std::to_string(cY), vec2{cursor_position.x, cursor_position.y}, true);
	}

	void destroy() {
		obstacle.destroy();

		for (Vehicle* v : vehicles) {
			v->destroy();
			delete v;
		}

		text.destroy_text();
	}
}