#include "..\include\simulation.h"

Simulation::Simulation() {
	index_state = 1;
	index_selected_vehicle = 0;

	num_lights = 3;
	
	follow_vehicle = false;
	mouse_pressed = false;
	is_running = false;

	cursor_position = vec2{ 0.f, 0.f };

	camera.field_of_view = 90.f;
	camera.target_distance = 30.f;
	camera.resolution = vec2{ 1366.f, 768.f };
	camera.depth_range_ortho = vec2{ -1.f, 1.f };
	camera.depth_range_persp = vec2{ 0.1f, 5000.f };
	camera.position_current = vec3{ 0.f, 0.f, 0.f };
	camera.position_start = vec3{ 0.f, 100.f, 0.f };
	camera.position_target = vec3{ 30.f, 30.f, 30.f };
	camera.orientation_up = vec3{ 0.f, 1.f, 0.f };
	camera.aspect_ratio = camera.resolution.x / camera.resolution.y;
	camera.matrix_view = shared::view_matrix(camera.position_current, camera.position_target, camera.orientation_up);
	camera.matrix_projection_ortho = maths::orthographic_matrix(camera.resolution, camera.depth_range_ortho.x, camera.depth_range_ortho.y, maths::mat4());
	camera.matrix_projection_persp = shared::perspective_matrix(camera.field_of_view, camera.aspect_ratio, camera.depth_range_persp.x, camera.depth_range_persp.y);

	ui = UI(camera);

	cube_renderer = Cube_Renderer();
	line_renderer = Line_Renderer();
	quad_renderer = Quad_Renderer();
	text_renderer = Text_Renderer(18, "data/ShareTechMono-Regular.ttf");
	model_renderer = Model_Renderer();
	circle_renderer = Circle_Renderer();

	wheel_model = Model();
	grid_model = Model();

	transforms_vehicles = vector<Transform>(10);
	attributes_vehicles = vector<Vehicle_Attributes>(10);

	for (int i = 0; i < transforms_vehicles.size(); i++) {
		transforms_vehicles[i] = {
			vec3{ utils::gen_random(-120.f, 120.f), 4.f, utils::gen_random(-120.f, 120.f) },
			vec3{ 16.f, 2.f, 8.f },
			vec3{ 0.f, utils::gen_random(0.f, 360.f), 0.f }
		};

		attributes_vehicles[i] = {
			utils::gen_random(0.2f, 0.5f),
			utils::gen_random(2.5f, 3.5f)
		};
	}

	// Init Physics
	physics = new Physics(transforms_vehicles.size(), transforms_vehicles);

	// Init Wheels
	attributes_wheels = vector<Wheel_Attributes>(4) = { { 315.f, 0.f },{ 225.f, 0.f },{ 45.f, 180.f },{ 135.f, 180.f } };
	transforms_wheels = vector<Transform>(transforms_vehicles.size() * 4);
	int index = 0;
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		for (int j = 0; j < 4; j++) {
			transforms_wheels[index] = attributes_wheels[j].gen_transform_from_vehicle(transforms_vehicles[i], 8.f);
			index++;
		}
	}

	// TODO Make these a param to renderers
	lights = new Light[num_lights];
	lights[0] = { 0.2f, {  0.f, 30.f,  0.f }, utils::colour::white };
	lights[1] = { 0.2f, { 10.f, 10.f, 10.f }, utils::colour::red };
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
	if (is_running) {
		physics->update();

		vector<vec2> positions = physics->get_vehicle_positions();
		vector<float> rotations = physics->get_vehicle_rotations();

		for (int i = 0; i < transforms_vehicles.size(); i++) {
			transforms_vehicles[i].rotation.y = rotations[i] + 90.f;
			transforms_vehicles[i].position = vec3{ positions[i].x, transforms_vehicles[i].position.y, positions[i].y };
		}
	}
		
	ui.update(cursor_position, mouse_pressed);
	camera.update(follow_vehicle, transforms_vehicles, index_selected_vehicle);

	mouse_pressed = false;
}

void Simulation::draw() {
	glEnable(GL_DEPTH_TEST);
	draw_walls();
	
	glEnable(GL_BLEND);
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		float y = i * 0.01f;
		circle_renderer.draw_3D_shadow(camera, { transforms_vehicles[i].position - vec3{ 0.f, 3.95f - y, 0.f },{ 20.f }, { 90.f, 0.f, 0.f } });
	}

	glDisable(GL_DEPTH_TEST);
	draw_ui();
	
	glDisable(GL_BLEND);
}

void Simulation::draw_vehicles() {
	// Draw the vehicle bodies
	cube_renderer.draw_multiple(transforms_vehicles.size(), camera, transforms_vehicles, utils::colour::blue);

	// Draw the sensors
	const static float SENSOR_ANGLE_OFFSET = 20.f;
	const static float SENSOR_RANGE = 40.f;
	int index = 0;
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		float a = transforms_vehicles[i].rotation.y - SENSOR_ANGLE_OFFSET;
		float b = transforms_vehicles[i].rotation.y + SENSOR_ANGLE_OFFSET;

		vec2 direction_a = polar_to_cartesian(to_radians(a)) * SENSOR_RANGE;
		vec2 direction_b = polar_to_cartesian(to_radians(b)) * SENSOR_RANGE;

		line_renderer.draw(
			camera,
			transforms_vehicles[i].position,
			transforms_vehicles[i].position + vec3{ direction_a.x, 0.f, direction_a.y },
			utils::colour::red
		);

		line_renderer.draw(
			camera,
			transforms_vehicles[i].position,
			transforms_vehicles[i].position + vec3{ direction_b.x, 0.f, direction_b.y },
			utils::colour::red
		);

		for (int j = 0; j < 4; j++) {
			transforms_wheels[index] = attributes_wheels[j].gen_transform_from_vehicle(transforms_vehicles[i], 8.f);
			index++;
		}

		model_renderer.draw_multiple_3D_textured(transforms_wheels.size(), wheel_model, camera, transforms_wheels, wheel_texture);
	}
}

void Simulation::draw_walls() {
	model_renderer.draw_3D_textured(grid_model, camera, { { 0.f, 200.f,  400.f },{ 40.f },{ 270.f, 0.f,   0.f } }, this->floor_texture);
	model_renderer.draw_3D_textured(grid_model, camera, { { 0.f, 200.f, -400.f },{ 40.f },{ -270.f, 0.f,   0.f } }, this->floor_texture);
	model_renderer.draw_3D_textured(grid_model, camera, { { 400.f, 200.f,    0.f },{ 40.f },{ 0.f, 0.f,  90.f } }, this->floor_texture);
	model_renderer.draw_3D_textured(grid_model, camera, { { -400.f, 200.f,    0.f },{ 40.f },{ 0.f, 0.f, -90.f } }, this->floor_texture);
	model_renderer.draw_3D_textured(grid_model, camera, { { 0.f,   0.f,    0.f },{ 40.f },{ 0.f, 0.f,   0.f } }, this->floor_texture);
	draw_vehicles();


	// TODO: CLEANUP!
	/*for (int i = 0; i < physics->wall_bodies.size(); i++) {
	float quad_y_rot = (physics->wall_bodies[i]->GetAngle() * (float)(180 / 3.141592f)) + 90.f;

	Transform t = {
	vec3{ physics->wall_bodies[i]->GetPosition().x, 10.f, physics->wall_bodies[i]->GetPosition().y },
	(i >= 2) ? vec3{ 4.f, 800.f, 0.f } : vec3{800.f, 4.f, 0.f},
	(i >= 2) ? vec3{ 90.f, quad_y_rot, 0.f } : vec3{0.f, quad_y_rot, 90.f}
	};

	quad_renderer.draw_3D_coloured(camera, t, utils::colour::red);
	}*/

	float quad_y_rot = box2d_to_simulation_angle(physics->wall_1->GetAngle());
	Transform t = {
		vec3{ physics->wall_1->GetPosition().x, 10.f, physics->wall_1->GetPosition().y },
		vec3{ 4.f, 800.f, 0.f },
		vec3{ 90.f, quad_y_rot, 0.f }
	};


	quad_y_rot = box2d_to_simulation_angle(physics->wall_2->GetAngle())
	t = {
		vec3{ physics->wall_2->GetPosition().x, 10.f, physics->wall_2->GetPosition().y },
		vec3{ 4.f, 800.f, 0.f },
		vec3{ 90.f, quad_y_rot, 0.f }
	};
	quad_renderer.draw_3D_coloured(camera, t, utils::colour::blue);


	quad_y_rot = box2d_to_simulation_angle(physics->wall_3->GetAngle())
	t = {
		vec3{ physics->wall_3->GetPosition().x, 10.f, physics->wall_3->GetPosition().y },
		vec3{ 4.f, 800.f, 0.f },
		vec3{ 0.f, quad_y_rot, 90.f }
	};
	quad_renderer.draw_3D_coloured(camera, t, utils::colour::green);

	quad_y_rot = box2d_to_simulation_angle(physics->wall_4->GetAngle())
	t = {
		vec3{ physics->wall_4->GetPosition().x, 10.f, physics->wall_4->GetPosition().y },
		vec3{ 800.f, 4.f, 0.f },
		vec3{ 0.f, quad_y_rot, 90.f }
	};
	quad_renderer.draw_3D_coloured(camera, t, utils::colour::yellow);

}

void Simulation::draw_ui() {
	if (ui.index_active_button != -1)
		quad_renderer.draw_2D(camera, ui.attributes_ui[ui.index_active_button].position, ui.attributes_ui[ui.index_active_button].size * 1.1f, utils::colour::yellow);

	for (int i = 0; i < ui.num_buttons; i++) {
		quad_renderer.draw_2D(camera, ui.attributes_ui[i].position, ui.attributes_ui[i].size, ui.attributes_ui[i].colour);
		text_renderer.draw(ui.attributes_ui[i].label, ui.attributes_ui[i].position + vec2{ 0.f, -5.f }, true, colour::white);
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
	physics->destroy();

	delete physics;
	delete[] lights;
}

void Simulation::add_vehicle() {
	Transform t = {
		vec3{ utils::gen_random(-120.f, 120.f), 4.f, utils::gen_random(-120.f, 120.f) },
		vec3{ 16.f, 2.f, 8.f },
		vec3{ 0.f, utils::gen_random(0.f, 360.f), 0.f }
	};

	Vehicle_Attributes av = {
		utils::gen_random(0.2f, 0.5f),
		utils::gen_random(2.5f, 3.5f)
	};

	transforms_vehicles.push_back(t);
	attributes_vehicles.push_back(av);

	transforms_wheels.push_back({});
	transforms_wheels.push_back({});
	transforms_wheels.push_back({});
	transforms_wheels.push_back({});

	physics->add_vehicle(t);
}

void Simulation::remove_vehicle() {
	if (transforms_vehicles.size() > 0) {
		transforms_vehicles.pop_back();
		attributes_vehicles.pop_back();
		for (int i = 0; i < 4; i++) {
			transforms_wheels.pop_back();
		}

		physics->remove_vehicle();
	}
}