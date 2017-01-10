#include "..\include\simulation.h"

Simulation::Simulation() {
	index_active_button = -1;
	index_pressed_button = -1;
	index_state = 1;
	index_selected_vehicle = 0;

	num_vehicles = 10;
	num_buttons = 6;
	num_lights = 3;
	
	follow_vehicle = false;
	mouse_pressed = false;
	cursor_position = vec2{ 0.f, 0.f };

	camera.field_of_view = 90.f;
	camera.target_distance = 20.f;
	camera.resolution = vec2{ 1366.f, 768.f };
	camera.depth_range_ortho = vec2{ -1.f, 1.f };
	camera.depth_range_persp = vec2{ 0.1f, 1000.f };
	camera.position_current = vec3{ 0.f, 0.f, 0.f };
	camera.position_start = vec3{ 0.f, 100.f, 0.f };
	camera.position_target = vec3{ 0.f, 0.f, 0.f };
	camera.orientation_up = vec3{ 0.f, 1.f, 0.f };
	camera.aspect_ratio = camera.resolution.x / camera.resolution.y;
	camera.matrix_view = shared::view_matrix(camera.position_current, camera.position_target, camera.orientation_up);
	camera.matrix_projection_ortho = maths::orthographic_matrix(camera.resolution, camera.depth_range_ortho.x, camera.depth_range_ortho.y, maths::mat4());
	camera.matrix_projection_persp = shared::perspective_matrix(camera.field_of_view, camera.aspect_ratio, camera.depth_range_persp.x, camera.depth_range_persp.y);

	cube_renderer = Cube_Renderer();
	line_renderer = Line_Renderer();
	quad_renderer = Quad_Renderer();
	text_renderer = Text_Renderer(18, "data/ShareTechMono-Regular.ttf");
	model_renderer = Model_Renderer();
	circle_renderer = Circle_Renderer();

	wheel_model = Model();
	grid_model = Model();

	vehicle_transforms = new utils::Transform[num_vehicles];
	vehicle_attributes = new Vehicle_Attributes[num_vehicles];
	for (int i = 0; i < num_vehicles; i++) {
		vehicle_transforms[i] = {
			vec3{ utils::gen_random(-120.f, 120.f), 4.f, utils::gen_random(-120.f, 120.f) },
			vec3{ 16.f, 2.f, 8.f },
			vec3{ 0.f, utils::gen_random(0.f, 360.f), 0.f }
		};

		vehicle_attributes[i] = {
			utils::gen_random(0.2f, 0.5f),
			utils::gen_random(2.5f, 3.5f)
		};
	}

	physics = new Physics(num_vehicles, vehicle_transforms);

	wheel_attributes = new Wheel_Attributes[4];
	wheel_attributes[0] = { 315.f, 0.f};
	wheel_attributes[1] = { 225.f, 0.f};
	wheel_attributes[2] = { 45.f, 180.f};
	wheel_attributes[3] = { 135.f, 180.f};

	std::string button_labels[6] = {
		"ADD VEHICLE", "REMOVE VEHICLE", "EDIT VEHICLE", "FOLLOW VEHICLE", "PLAY", "PAUSE"
	};

	button_attributes = new Button_Attributes[num_buttons];
	for (int i = 0; i < num_buttons; i++) {
		float width_by_buttons = camera.resolution.x / num_buttons;
		float p = (i * width_by_buttons) + (width_by_buttons * 0.5f);
		button_attributes[i] = { { p, 740.f }, { 206.f, 32.f }, utils::data::colour::dark_grey, button_labels[i]};
	}

	// TODO Make these a param to renderers
	lights = new Light[num_lights];
	lights[0] = { 0.2f, {  0.f, 30.f,  0.f }, utils::data::colour::white };
	lights[1] = { 0.2f, { 10.f, 10.f, 10.f }, utils::data::colour::red };
}

void Simulation::init() {
	wheel_texture.init("data/wheel_texture.png");
	floor_texture.init("data/floor.png");

	wheel_model.init("data/wheel.obj");
	grid_model.init("data/grid.obj");

	cube_renderer.init();
	line_renderer.init();
	quad_renderer.init();
	text_renderer.init(camera.resolution);
	circle_renderer.init();
	model_renderer.init();
}

void Simulation::update() {
	physics->update();
	
	vector<vec2> positions = physics->get_vehicle_positions();
	vector<float> rotations = physics->get_vehicle_rotations();

	for (int i = 0; i < num_vehicles; i++) {
		vehicle_transforms[i].rotation.y = rotations[i] + 90.f;
		vehicle_transforms[i].position = vec3{ positions[i].x, vehicle_transforms[i].position.y, positions[i].y };
	}
		
	update_ui();
	update_camera();

	mouse_pressed = false;
}

void Simulation::update_camera() {
	if (follow_vehicle) {
		vec2 direction = polar_to_cartesian(to_radians(vehicle_transforms[index_selected_vehicle].rotation.y));
		direction *= camera.target_distance;

		camera.position_current = vehicle_transforms[index_selected_vehicle].position;
		camera.position_current.x -= direction.x;
		camera.position_current.y += camera.target_distance;
		camera.position_current.z -= direction.y;
		camera.position_target = camera.position_current + vec3{ direction.x, -camera.target_distance, direction.y };
	}
	else {
		camera.position_current = camera.position_start;
		camera.position_target = vehicle_transforms[index_selected_vehicle].position;
	}

	camera.matrix_projection_persp = shared::perspective_matrix(camera.field_of_view, camera.aspect_ratio, camera.depth_range_persp.x, camera.depth_range_persp.y);
	camera.matrix_view = shared::view_matrix(camera.position_current, camera.position_target, camera.orientation_up);
}

void Simulation::update_ui() {
	index_active_button = -1;
	index_pressed_button = -1;
	for (int i = 0; i < num_buttons; i++) {
		float l = button_attributes[i].position.x - (button_attributes[i].size.x * 0.5f);
		float r = button_attributes[i].position.x + (button_attributes[i].size.x * 0.5f);
		float u = button_attributes[i].position.y + (button_attributes[i].size.y * 0.5f);
		float d = button_attributes[i].position.y - (button_attributes[i].size.y * 0.5f);
		if (utils::point_quad_intersect(cursor_position, l, r, u, d))
			index_active_button = i;
	}

	if (mouse_pressed && index_active_button != -1)
		index_pressed_button = index_active_button;
}

void Simulation::draw() {
	glEnable(GL_DEPTH_TEST);
	
	draw_environment();
	draw_vehicles();

	glEnable(GL_BLEND);

	// Draw the shadows
	for (int i = 0; i < num_vehicles; i++) {
		// Fudge the Y position by a tiny bit to prevent z-fighting
		float y = i * 0.01f;
		circle_renderer.draw_3D_shadow(camera, { vehicle_transforms[i].position - vec3{ 0.f, 3.95f - y, 0.f },{ 20.f }, { 90.f, 0.f, 0.f } });
	}

	glDisable(GL_DEPTH_TEST);

	draw_ui();

	glDisable(GL_BLEND);
}

void Simulation::draw_vehicles() {
	// Draw the vehicles
	cube_renderer.draw_multiple(num_vehicles, camera, vehicle_transforms, utils::data::colour::blue);


	// Draw the wheels
	int num_wheels = num_vehicles * 4;
	Transform* transform_list = new Transform[num_wheels];
	int index = 0;
	for (int i = 0; i < num_vehicles; i++) {
		for (int j = 0; j < 4; j++) {
			transform_list[index] = wheel_attributes[j].gen_transform_from_vehicle(vehicle_transforms[i], 8.f);
			index++;
		}
	}

	model_renderer.draw_multiple_3D_textured(num_wheels, wheel_model, camera, transform_list, wheel_texture);

	delete[] transform_list;

}

void Simulation::draw_environment() {
	model_renderer.draw_3D_textured(grid_model, camera, { { 0.f, 0.f, 0.f },{ 40.f },{ 0.f, 0.f, 0.f } }, this->floor_texture);
}

void Simulation::draw_ui() {
	if (index_active_button != -1) {
		quad_renderer.draw_2D(camera, button_attributes[index_active_button].position, button_attributes[index_active_button].size * 1.1f, utils::data::colour::yellow);
	}

	for (int i = 0; i < num_buttons; i++) {
		quad_renderer.draw_2D(camera, button_attributes[i].position, button_attributes[i].size, button_attributes[i].colour);
		text_renderer.draw(button_attributes[i].label, button_attributes[i].position + vec2{ 0.f, -5.f }, true, data::colour::white);
	}

	if (index_pressed_button != -1) {
		vec2 position = { button_attributes[index_active_button].position.x, button_attributes[index_active_button].position.y - 50 };
		text_renderer.draw("go", position, true, data::colour::white);
	}
}

void Simulation::destroy() {
	cube_renderer.destroy();
	line_renderer.destroy();
	quad_renderer.destroy();
	text_renderer.destroy();
	circle_renderer.destroy();
	model_renderer.destroy();

	wheel_texture.destroy();
	floor_texture.destroy();

	wheel_model.destroy();
	grid_model.destroy();

	delete physics;

	delete[] vehicle_transforms;
	delete[] vehicle_attributes;
	delete[] button_attributes;
	delete[] wheel_attributes;
	delete[] lights;
}