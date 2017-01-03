#include "..\include\simulation.h"

Simulation::Simulation() {
	index_active_button = -1;
	index_pressed_button = -1;
	index_state = 1;
	index_selected_vehicle = 0;

	num_vehicles = 25;
	num_buttons = 6;
	
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
	mesh_renderer = Mesh_Renderer();
	circle_renderer = Circle_Renderer();
	model = Model();

	vehicle_transforms = new utils::Transform[num_vehicles];
	vehicle_attributes = new Vehicle_Attributes[num_vehicles];
	for (int i = 0; i < num_vehicles; i++) {
		vehicle_transforms[i] = {
			vec3{ utils::gen_random(-90.f, 90.f), 4.f, utils::gen_random(-90.f, 90.f) },
			vec3{ 16.f, 2.f, 8.f },
			vec3{ 0.f, utils::gen_random(0.f, 360.f), 0.f }
		};

		vehicle_attributes[i] = {
			utils::gen_random(0.2f, 0.5f),
			utils::gen_random(2.5f, 3.5f)
		};
	}

	wheel_attributes = new Wheel_Attributes[4];
	wheel_attributes[0] = { 315.f, 0.f};
	wheel_attributes[1] = { 225.f, 0.f};
	wheel_attributes[2] = { 45.f, 180.f};
	wheel_attributes[3] = { 135.f, 180.f};

	button_attributes = new Button_Attributes[num_buttons];
	button_attributes[0] = { { 100.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "add vehicle" };
	button_attributes[1] = { { 300.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "remove vehicle" };
	button_attributes[2] = { { 500.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "follow vehicle" };
	button_attributes[3] = { { 700.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "pause" };
	button_attributes[4] = { { 900.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "play" };
	button_attributes[5] = { { 1100.f, 700.f },{ 192.f, 32.f },{ 0.1f, 0.2f, 0.1f, 1.f }, "edit vehicle" };
}

void Simulation::init() {
	wheel_texture.init("data/wheel_texture.png");
	floor_texture.init("data/floor.png");
	model.init("data/wheel.obj");

	cube_renderer.init();
	line_renderer.init();
	quad_renderer.init();
	text_renderer.init(camera.resolution);
	circle_renderer.init();
	mesh_renderer.init(model);
}

void Simulation::update() {
	if (utils::elapsed_time() > 10)
		follow_vehicle = true;

	for (int i = 0; i < num_vehicles; i++)
		update_vehicle(vehicle_transforms[i], vehicle_attributes[i]);
		
	update_ui();
	update_camera();

	mouse_pressed = false;
}

void Simulation::update_vehicle(utils::Transform& transform, Vehicle_Attributes attribs) {
	if (!point_quad_intersect(transform.position.XZ(), -100, 100, 100, -100))
		transform.rotation.y += attribs.turning_speed;

	vec2 direction = polar_to_cartesian(to_radians(transform.rotation.y));
	vec2 velocity = direction * attribs.forward_speed;
	transform.position += {velocity.x, 0.f, velocity.y};
}

void Simulation::update_camera() {
	if (follow_vehicle) {
		vec2 direction = polar_to_cartesian(to_radians(vehicle_transforms[index_selected_vehicle].rotation.y));
		direction *= camera.target_distance;

		camera.position_current = vehicle_transforms[index_selected_vehicle].position;
		camera.position_current.x -= direction.x;
		camera.position_current.y += camera.target_distance;
		camera.position_current.z -= direction.y;
		camera.position_target = camera.position_current + vec3{ direction.x, -20.f, direction.y };
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
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	draw_ui();
	glDisable(GL_BLEND);
}

void Simulation::draw_vehicles() {
	// Draw the vehicles
	cube_renderer.draw_multiple(num_vehicles, camera, vehicle_transforms, utils::data::colour::blue);

	// Draw the wheels
	for (int i = 0; i < num_vehicles; i++) {
		for (int j = 0; j < 4; j++) {
			Transform t = wheel_attributes[j].gen_transform_from_vehicle(vehicle_transforms[i], 8.f);
			mesh_renderer.draw_3D_textured(model, camera, t, wheel_texture);
		}

		quad_renderer.draw_3D_textured(camera, { vehicle_transforms[i].position - vec3{ 0.f, 3.9f, 0.f }, { 18.f }, { 90.f, 0.f, 0.f } }, shadow_texture);
	}
}

void Simulation::draw_environment() {
	quad_renderer.draw_3D_textured(camera, { { 0.f, 0.f, 0.f }, { 400.f }, { 90.f, 0.f, 0.f } }, floor_texture);
}

void Simulation::draw_ui() {
	for (int i = 0; i < num_buttons; i++) {
		quad_renderer.draw_2D(camera, button_attributes[i].position, button_attributes[i].size, button_attributes[i].colour);
		text_renderer.draw(button_attributes[i].label, button_attributes[i].position, true, data::colour::white);
	}

	if (index_active_button != -1) {
		text_renderer.draw("wow", button_attributes[index_active_button].position, true, data::colour::white);
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
	mesh_renderer.destroy();

	wheel_texture.destroy();
	floor_texture.destroy();
	shadow_texture.destroy();

	delete[] vehicle_transforms;
	delete[] vehicle_attributes;
	delete[] button_attributes;
	delete[] wheel_attributes;
}