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

		Transform vehicle_transform = Transform{vec2{500.f, 550.f}, vec2{100.f, 50.f}, 0.f};
		vehicle = Vehicle{vehicle_transform, vec4{1.f, 1.f, 1.f, 1.f}, 0.002f};

		text = Text{24, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
	
		boundary.init();
		obstacle.init();
		vehicle.init();
		text.init_text(resolution);

		state = 1;
	}

	void update() {
		

		if (state == 1) {
			boundary.update(cursor_position);
			obstacle.update(cursor_position);
			vehicle.update(cursor_position);
		}
	}

	void draw() {
		boundary.draw();
		obstacle.draw();
		vehicle.draw();

		text.draw_text("  CURSOR: Obstacle", vec2(1366.f * 0.5f, 128.f), true);
		text.draw_text(" VEHICLE: Excitory", vec2{1366.f * 0.5f, 64.f}, true);
	}

	void destroy() {
		obstacle.destroy();
		vehicle.destroy();	
		text.destroy_text();
	}
}




//	{ // Middle Col
//		float left = 450.f;
//		float alpha4 = 0.4f * sin(glfwGetTime() * 4.f) + 0.6f;
//		float alpha8 = 0.4f * sin(glfwGetTime() * 16.f) + 0.6f;

//		text.colour = vec4{1.f, 1.f, 1.f, 1.f};
//		text.draw_text(" LEFT SENSOR:", vec2(left , text_pos_y + vertical_text_pos_offset * 3));
//		text.draw_text("RIGHT SENSOR:", vec2(left, text_pos_y + vertical_text_pos_offset * 2));

//						
//		if (vehicle.left_sensor.detected_object && state == 1) {
//			text.colour = vec4{0.f, 1.f, 0.f, alpha8};
//			text.draw_text("<detected>", vec2(640.f, text_pos_y + vertical_text_pos_offset * 3));
//		} else {
//			text.colour = vec4{1.f, 0.f, 0.f, alpha4};
//			text.draw_text("<seeking>", vec2(640.f, text_pos_y + vertical_text_pos_offset * 3));
//		}

//		if (vehicle.right_sensor.detected_object && state == 1) {
//			text.colour = vec4{0.f, 1.f, 0.f, alpha8};
//			text.draw_text("<detected>", vec2(640.f, text_pos_y + vertical_text_pos_offset * 2));
//		} else {
//			text.colour = vec4{1.f, 0.f, 0.f, alpha4};
//			text.draw_text("<seeking>", vec2(640.f, text_pos_y + vertical_text_pos_offset * 2));
//		}
//	}
//	
//	{ // Right Col
//		float left = 900.f;
//		std::vector<std::string> attribs = vehicle.string_attribs();

//		text.colour = vec4{1.f, 1.f, 1.f, 1.f};
//		for (int i = 0; i < attribs.size(); ++i) {
//			text.draw_text(attribs.at(i), vec2{left, text_pos_y + vertical_text_pos_offset * i});
//		}
//	}

//	text.colour = vec4{0.f, 1.f, 1.f, 1.f};
//	int cX = cursor_position.x;
//	int cY = cursor_position.y;

//	text.draw_text(std::to_string(cX) + ", " + std::to_string(cY), vec2{cursor_position.x, cursor_position.y - 32.f});			
//}