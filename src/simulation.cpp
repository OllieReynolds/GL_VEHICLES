#include "..\include\simulation.h"

namespace simulation {
	vec2 resolution;
	vec2 near_far;
	mat4 resolution_matrix;
	vec2 cursor_position;

	Boundary boundary;
	Obstacle obstacle;
	Vehicle vehicle;

	Text text;

	void init() {
		cursor_position   = vec2{0.f, 0.f};
		resolution        = vec2{1366.f, 768.f};
		near_far          = vec2{-1.f, 1.f};
		resolution_matrix = orthographic_matrix(resolution, near_far.x, near_far.y, maths::mat4());

		Transform boundary_transform = Transform{vec2{resolution.x * 0.5f, resolution.y * 0.6f}, vec2{resolution.x * 0.95f, resolution.y * 0.75f}, 0.f};
		boundary = Boundary{boundary_transform, vec4{1.f, 1.f, 1.f, 0.05f}};

		Transform obstacle_transform = Transform{vec2{600.f, 450}, vec2{16.f, 16.f}, 0.f};
		obstacle = Obstacle{obstacle_transform, vec4{1.f, 1.f, 1.f, 0.7f}};

		Transform vehicle_transform = Transform{vec2{900.f, 550.f}, vec2{50.f, 50.f}, 45.f};
		vehicle = Vehicle{vehicle_transform, vec4{1.f, 1.f, 1.f, 1.f}, 45.f, 0.002f};

		text = Text{24, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
	
		boundary.init();
		obstacle.init();
		vehicle.init();
		text.init_text();
	}

	void update() {
		boundary.update(cursor_position);
		obstacle.update(cursor_position);

		vehicle.update(cursor_position);
	}

	void draw() {
		boundary.draw();

		obstacle.draw();

		vehicle.draw();

		{ // Draw the GUI
			float text_pos_y = 32.f;
			float vertical_text_pos_offset = 32.f;
			
			{ // Left Col
				float left = 34.f;
				text.colour = vec4{1.f, 1.f, 1.f, 1.f};

				text.draw_text("  CURSOR: Obstacle", vec2(left, text_pos_y + vertical_text_pos_offset * 3));
				text.draw_text(" VEHICLE: Excitory", vec2{left, text_pos_y + vertical_text_pos_offset * 2});
				text.draw_text("FRICTION: Disabled", vec2{left, text_pos_y + vertical_text_pos_offset * 1});
			}

			
			{ // Middle Col
				float left = 450.f;
				float alpha = 0.4f * sin(glfwGetTime() * 4.f) + 0.6f;

				text.colour = vec4{1.f, 1.f, 1.f, 1.f};
				text.draw_text(" LEFT SENSOR:", vec2(left , text_pos_y + vertical_text_pos_offset * 3));
				text.draw_text("RIGHT SENSOR:", vec2(left, text_pos_y + vertical_text_pos_offset * 2));

				text.colour = vec4{1.f, 1.f, 0.f, alpha};
				text.draw_text("<seeking>", vec2(640.f, text_pos_y + vertical_text_pos_offset * 3));
				text.draw_text("<seeking>", vec2(640.f, text_pos_y + vertical_text_pos_offset * 2));
			}
			
			{ // Right Col
				float left = 900.f;
				std::vector<std::string> attribs = vehicle.string_attribs();

				text.colour = vec4{1.f, 1.f, 1.f, 1.f};
				for (int i = 0; i < attribs.size(); ++i) {
					text.draw_text(attribs.at(i), vec2{left, text_pos_y + vertical_text_pos_offset * i});
				}
			}

			
			
		}
	}

	void destroy() {
		obstacle.destroy();

		vehicle.destroy();	

		text.destroy_text();
	}
}
