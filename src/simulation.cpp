#include "..\include\simulation.h"

namespace simulation {
	vec2 resolution;
	vec2 near_far;
	mat4 resolution_matrix;
	vec2 cursor_position;

	Obstacle obstacle;
	Vehicle vehicle;

	Text text;

	void init() {
		cursor_position   = vec2{0.f, 0.f};
		resolution        = vec2{1366.f, 768.f};
		near_far          = vec2{-1.f, 1.f};
		resolution_matrix = orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());

		obstacle = Obstacle{
			vec4{1.f, 1.f, 1.f, 1.f}, // Colour
			vec2{600.f, 450},         // Position
			vec2{32.f, 32.f},         // Size
			0.f                       // Rotation
		};

		vehicle = Vehicle{
			vec2{900.f, 550.f},       // Position
			vec2{100.f, 100.f},       // Size
			vec2{50.f, 50.f},         // Sensor Offset
			0.f,                      // Rotation
			0.1f,                     // Speed
			0.002f                    // Turning force
		};

		text = Text{64};
	

		vehicle.init();
		obstacle.init();
		text.init_text();
	}

	void update() {
		obstacle.update(cursor_position);

		vehicle.set_obstacle_ptr(obstacle);
		vehicle.update(cursor_position);
	}

	void draw() {
		obstacle.draw();

		vehicle.draw();

		text.draw_text("ACTIVE VEHICLE: EXCITORY", vec2{10.f, 10.f});
	}

	void destroy() {
		obstacle.destroy();

		vehicle.destroy();	

		text.destroy_text();
	}
}
