#include "..\include\simulation.h"

Simulation::Simulation() {
	index_state = 1;
	index_selected_vehicle = 0;

	mouse_pressed = false;
	is_updating = false;
	is_drawing = true;

	ui = UI(camera);
	text_renderer = Text_Renderer(18, "data/ShareTechMono-Regular.ttf");


	// Init Physics
	physics = new Physics(transforms_vehicles.size(), transforms_vehicles);
	vector<vec2> positions = physics->get_vehicle_positions();
	vector<float> rotations = physics->get_vehicle_rotations();
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		transforms_vehicles[i].rotation.y = rotations[i] + 90.f;
		transforms_vehicles[i].position = vec3{ positions[i].x, transforms_vehicles[i].position.y, positions[i].y };
	}

	// Init Vehicles
	for (int i = 0; i < 6; i++) {
		bool is_predator = i % 2 == 0;
		add_vehicle(is_predator);
	}


	attributes_wheels = vector<Wheel_Attributes>(4) = { { 315.f, 0.f },{ 225.f, 0.f },{ 45.f, 180.f },{ 135.f, 180.f } };

	// Init Walls
	transforms_walls = vector<Transform>(5);
	transforms_walls[0] = { vec3{    0.f, 200.f,  400.f }, vec3{ 40.f }, vec3{  270.f, 0.f,   0.f } };
	transforms_walls[1] = { vec3{    0.f, 200.f, -400.f }, vec3{ 40.f }, vec3{ -270.f, 0.f,   0.f } };
	transforms_walls[2] = { vec3{  400.f, 200.f,    0.f }, vec3{ 40.f }, vec3{    0.f, 0.f,  90.f } };
	transforms_walls[3] = { vec3{ -400.f, 200.f,    0.f }, vec3{ 40.f }, vec3{    0.f, 0.f, -90.f } };
	transforms_walls[4] = { vec3{    0.f,   0.f,    0.f }, vec3{ 40.f }, vec3{    0.f, 0.f,   0.f } };

	// Init Boundaries 
	transforms_boundaries = vector<Transform>(4);
	transforms_boundaries[0] = { vec3{ physics->wall_1->GetPosition().x, 10.f, physics->wall_1->GetPosition().y }, vec3{ 4.f, 800.f, 0.f }, vec3{ 90.f, box2d_to_simulation_angle(physics->wall_1->GetAngle()), 0.f } };
	transforms_boundaries[1] = { vec3{ physics->wall_2->GetPosition().x, 10.f, physics->wall_2->GetPosition().y }, vec3{ 4.f, 800.f, 0.f }, vec3{ 90.f, box2d_to_simulation_angle(physics->wall_2->GetAngle()), 0.f } };
	transforms_boundaries[2] = { vec3{ physics->wall_3->GetPosition().x, 10.f, physics->wall_3->GetPosition().y }, vec3{ 4.f, 800.f, 0.f }, vec3{  0.f, box2d_to_simulation_angle(physics->wall_3->GetAngle()), 90.f } };
	transforms_boundaries[3] = { vec3{ physics->wall_4->GetPosition().x, 10.f, physics->wall_4->GetPosition().y }, vec3{ 4.f, 800.f, 0.f }, vec3{  0.f, box2d_to_simulation_angle(physics->wall_4->GetAngle()), 90.f } };

	// Init Shadows
	transforms_shadows = vector<Transform>(transforms_vehicles.size());
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		transforms_shadows[i] = { transforms_vehicles[i].position - vec3{ 0.f, 3.95f - i * 0.01f, 0.f },{ 20.f }, { 90.f, 0.f, 0.f } };
	}

	// Init Lights
	lights = vector<Light>();
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		lights.push_back({ { 0.f, 30.f, 0.f }, attributes_vehicles[i].colour.XYZ() });
	}
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
	tri_renderer.init();
}

void Simulation::update() {

	for (int i = 0; i < lights.size(); i++) {
		lights[i].position = transforms_vehicles[i].position;
	}

	if (is_updating) {
		// Update Physics
		physics->update();

		// Vehicles Transforms
		update_simulation_transforms_from_physics();
		update_sensors_from_simulation_transforms();

		// Shadows
		for (int i = 0; i < transforms_shadows.size(); i++) {
			transforms_shadows[i].position.x = transforms_vehicles[i].position.x;
			transforms_shadows[i].position.z = transforms_vehicles[i].position.z;
		}

		
		check_detected_vehicles();
		//check_detected_walls();
		predator_prey();
		
	}

	ui.update(cursor_position, mouse_pressed);
	camera.update(transforms_vehicles, index_selected_vehicle);

	mouse_pressed = false;
}

void Simulation::draw() {
	if (is_drawing) {
		glEnable(GL_DEPTH_TEST);
		{
			// Walls & Floor
			model_renderer.draw_multiple_3D_textured(transforms_walls.size(), grid_model, camera, transforms_walls, floor_texture, lights);

			// Boundaries
			quad_renderer.draw_multiple_3D_coloured(camera, transforms_boundaries, utils::colour::red);

			if (!transforms_vehicles.empty()) {
				// Vehicles
				cube_renderer.draw_multiple(camera, transforms_vehicles, attributes_vehicles, lights);

				// Wheels
				model_renderer.draw_multiple_3D_textured(transforms_wheels.size(), wheel_model, camera, transforms_wheels, wheel_texture, lights);
			}
		}

	
		glEnable(GL_BLEND);
		{
			if (!transforms_vehicles.empty()) {
				// Shadows
				circle_renderer.draw_multiple_3D_shadow(camera, transforms_shadows);

				// Vehicle Sensors
				for (int i = 0; i < vehicle_sensors.size(); i++) {
					if (vehicle_sensors[i].ldetected)
						tri_renderer.draw_3D_coloured(camera, vehicle_sensors[i].la, vehicle_sensors[i].lb, vehicle_sensors[i].lc, vec4{ 1.f, 1.f, 0.f, 0.01f });
					else
						tri_renderer.draw_3D_coloured(camera, vehicle_sensors[i].la, vehicle_sensors[i].lb, vehicle_sensors[i].lc, vec4{ 1.f, 0.f, 0.f, 0.01f });

					if (vehicle_sensors[i].rdetected)
						tri_renderer.draw_3D_coloured(camera, vehicle_sensors[i].ra, vehicle_sensors[i].rb, vehicle_sensors[i].rc, vec4{ 1.f, 1.f, 0.f, 0.01f });
					else
						tri_renderer.draw_3D_coloured(camera, vehicle_sensors[i].ra, vehicle_sensors[i].rb, vehicle_sensors[i].rc, vec4{ 1.f, 0.f, 0.f, 0.01f });
				}
			}
		}

		glDisable(GL_DEPTH_TEST);
		{
			// UI
			if (ui.index_active_button != -1)
				quad_renderer.draw_2D(camera, ui.attributes_ui[ui.index_active_button].position, ui.attributes_ui[ui.index_active_button].size * 1.1f, utils::colour::yellow);
			for (int i = 0; i < ui.attributes_ui.size(); i++) {
				quad_renderer.draw_2D(camera, ui.attributes_ui[i].position, ui.attributes_ui[i].size, ui.attributes_ui[i].colour);
				text_renderer.draw(ui.attributes_ui[i].label, ui.attributes_ui[i].position + vec2{ 0.f, -5.f }, true, colour::white);
			}
			text_renderer.draw(" Predators: 10", { 0.f, 50.f }, false, utils::colour::white);
			text_renderer.draw("      Prey: 10", { 0.f, 30.f }, false, utils::colour::white);
			text_renderer.draw("Generation:  0", { 0.f, 10.f }, false, utils::colour::white);
		}

		glDisable(GL_BLEND);
	}
}


void Simulation::destroy() {
	cube_renderer.destroy();
	line_renderer.destroy();
	quad_renderer.destroy();
	text_renderer.destroy();
	circle_renderer.destroy();
	model_renderer.destroy();
	tri_renderer.destroy();
	wheel_texture.destroy();
	floor_texture.destroy();
	wheel_model.destroy();
	grid_model.destroy();
	physics->destroy();

	delete physics;
}

void Simulation::add_vehicle(bool is_predator) {
	vec2 rand_pos = { utils::gen_random(-320.f, 320.f),  utils::gen_random(-320.f, 320.f) };
	vec2 from_origin = vec2{ 0.f, 0.f } -rand_pos;
	float angle = to_degrees( cartesian_to_polar(from_origin));

	Transform t = {
		vec3{ rand_pos.x, 4.f, rand_pos.y },
		vec3{ 16.f, 2.f, 8.f },
		vec3{ 0.f, angle, 0.f }
	};

	Vehicle_Attributes av = {
		utils::gen_random(0.2f, 0.5f),
		utils::gen_random(2.5f, 3.5f),
		(is_predator) ? utils::colour::red : utils::colour::blue,
		is_predator
	};

	
	transforms_vehicles.push_back(t);
	attributes_vehicles.push_back(av);

	lights.push_back({ { 0.f, 30.f, 0.f }, attributes_vehicles.back().colour.XYZ() });
	 
	transforms_shadows.push_back({ t.position - vec3{ 0.f, 3.95f, 0.f }, vec3{ 20.f }, vec3{ 90.f, 0.f, 0.f } });

	transforms_wheels.push_back({});
	transforms_wheels.push_back({});
	transforms_wheels.push_back({});
	transforms_wheels.push_back({});

	static const float SENSOR_ANGLE = 60.f;
	static const float SENSOR_OFFSET = 20.f;
	static const float SENSOR_RANGE = 400.f;
	{
		float y = transforms_vehicles.back().position.y - 6.f + ((vehicle_sensors.size() + 1) * 0.8f);

		float _a = t.rotation.y - SENSOR_OFFSET;
		vec2 a_left = polar_to_cartesian(to_radians(_a - SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec2 a_right = polar_to_cartesian(to_radians(_a + SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec3 la = t.position + vec3{ a_left.x, y, a_left.y };
		vec3 lb = t.position + vec3{ 0.f, y, 0.f };
		vec3 lc = t.position + vec3{ a_right.x, y, a_right.y };

		y = transforms_vehicles.back().position.y - 6.f + ((vehicle_sensors.size() + 2) * 0.8f);

		float _b = t.rotation.y + SENSOR_OFFSET;
		vec2 b_left = polar_to_cartesian(to_radians(_b - SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec2 b_right = polar_to_cartesian(to_radians(_b + SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec3 ra = t.position + vec3{ b_left.x, y, b_left.y };
		vec3 rb = t.position + vec3{ 0.f, y, 0.f };
		vec3 rc = t.position + vec3{ b_right.x, y, b_right.y };

		vehicle_sensors.push_back(Vehicle_Sensors{ la, lb, lc, ra, rb, rc, false, false });
	}

	physics->add_vehicle(t);
}

void Simulation::remove_vehicle() {
	if (!transforms_vehicles.empty()) {
		transforms_vehicles.pop_back();
		transforms_shadows.pop_back();
		attributes_vehicles.pop_back();

		for (int i = 0; i < 4; i++) {
			transforms_wheels.pop_back();
		}

		lights.pop_back();

		vehicle_sensors.pop_back();

		physics->remove_vehicle();

		if (transforms_vehicles.empty())
			camera.follow_vehicle = false;
	}
}

void Simulation::reset() {
	int n = transforms_vehicles.size();

	for (int i = 0; i < n; i++) {
		remove_vehicle();
	}

	for (int i = 0; i < n; i++) {
		add_vehicle(gen_random(0.f, 1.f) > 0.5f);
	}

	index_selected_vehicle = 0;
}

void Simulation::check_detected_walls() {
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		vec2 a = vehicle_sensors[i].la.XZ();
		vec2 b = vehicle_sensors[i].lb.XZ();
		vec2 c = vehicle_sensors[i].lc.XZ();

		vec2 p0 = { -400.f, -400.f };
		vec2 p1 = { -400.f,  400.f };
		vec2 p2 = { 400.f,  400.f };
		vec2 p3 = { 400.f, -400.f };

		if (
			utils::shared::Intersecting(p0, p1, a, b, c) ||
			utils::shared::Intersecting(p1, p2, a, b, c) ||
			utils::shared::Intersecting(p2, p3, a, b, c) ||
			utils::shared::Intersecting(p3, p0, a, b, c)
			) {
			vehicle_sensors[i].ldetected = true;
			physics->vehicles[i].control_state = DOWN | RIGHT;
		}


		a = vehicle_sensors[i].ra.XZ();
		b = vehicle_sensors[i].rb.XZ();
		c = vehicle_sensors[i].rc.XZ();

		if (
			utils::shared::Intersecting(p0, p1, a, b, c) ||
			utils::shared::Intersecting(p1, p2, a, b, c) ||
			utils::shared::Intersecting(p2, p3, a, b, c) ||
			utils::shared::Intersecting(p3, p0, a, b, c)
			) {
			vehicle_sensors[i].rdetected = true;
			physics->vehicles[i].control_state = DOWN | LEFT;
		}
	}
}

void Simulation::update_simulation_transforms_from_physics() {
	vector<vec2> positions = physics->get_vehicle_positions();
	vector<float> rotations = physics->get_vehicle_rotations();

	int index = 0;
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		transforms_vehicles[i].rotation.y = rotations[i] + 90.f;
		transforms_vehicles[i].position = vec3{ positions[i].x, transforms_vehicles[i].position.y, positions[i].y };

		for (int j = 0; j < 4; j++) {
			transforms_wheels[index] = attributes_wheels[j].gen_transform_from_vehicle(physics->vehicles[i].body->GetLinearVelocity(), transforms_vehicles[i], 8.f);
			index++;
		}
	}
}

void Simulation::update_sensors_from_simulation_transforms() {
	// Update Vehicle Sensor Transforms
	static const float SENSOR_ANGLE = 70.f;
	static const float SENSOR_OFFSET = 25.f;
	static const float SENSOR_RANGE = 300.f;

	int sensor_num = 0;
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		float y = transforms_vehicles[i].position.y - 6.f + (sensor_num++ * .8f);
		float _a = transforms_vehicles[i].rotation.y - SENSOR_OFFSET;
		vec2 a_left = polar_to_cartesian(to_radians(_a - SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec2 a_right = polar_to_cartesian(to_radians(_a + SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec3 la = transforms_vehicles[i].position + vec3{ a_left.x, y, a_left.y };
		vec3 lb = transforms_vehicles[i].position + vec3{ 0.f, y, 0.f };
		vec3 lc = transforms_vehicles[i].position + vec3{ a_right.x, y, a_right.y };

		y = transforms_vehicles[i].position.y - 6.f + (sensor_num++ * .8f);
		float _b = transforms_vehicles[i].rotation.y + SENSOR_OFFSET;
		vec2 b_left = polar_to_cartesian(to_radians(_b - SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec2 b_right = polar_to_cartesian(to_radians(_b + SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec3 ra = transforms_vehicles[i].position + vec3{ b_left.x, y, b_left.y };
		vec3 rb = transforms_vehicles[i].position + vec3{ 0.f, y, 0.f };
		vec3 rc = transforms_vehicles[i].position + vec3{ b_right.x, y, b_right.y };

		vehicle_sensors[i] = Vehicle_Sensors{ la, lb, lc, ra, rb, rc, false, false, false };
	}
}

void Simulation::check_detected_vehicles() {
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		for (int j = 0; j < transforms_vehicles.size(); j++) {
			if (i != j) {
				vec2 p = transforms_vehicles[j].position.XZ();

				vec2 p1 = p + vec2{ -10.f, -10.f };
				vec2 p2 = p + vec2{ 10.f, -10.f };
				vec2 p3 = p + vec2{ -10.f,  10.f };
				vec2 p4 = p + vec2{ 10.f,  10.f };

				vec2 a = vehicle_sensors[i].la.XZ();
				vec2 b = vehicle_sensors[i].lb.XZ();
				vec2 c = vehicle_sensors[i].lc.XZ();
				if (point_triangle_intersect(p1, a, b, c) || point_triangle_intersect(p2, a, b, c) ||
					point_triangle_intersect(p3, a, b, c) || point_triangle_intersect(p4, a, b, c))
				{
					vehicle_sensors[i].ldetected = true;
					vehicle_sensors[i].detected_predator = attributes_vehicles[j].is_predator;
				}

				a = vehicle_sensors[i].ra.XZ();
				b = vehicle_sensors[i].rb.XZ();
				c = vehicle_sensors[i].rc.XZ();
				if (point_triangle_intersect(p1, a, b, c) || point_triangle_intersect(p2, a, b, c) ||
					point_triangle_intersect(p3, a, b, c) || point_triangle_intersect(p4, a, b, c))
				{
					vehicle_sensors[i].rdetected = true;
					vehicle_sensors[i].detected_predator = attributes_vehicles[j].is_predator;
				}
			}
		}
	}
}

void Simulation::predator_prey() {
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		if (attributes_vehicles[i].is_predator) {

			if (vehicle_sensors[i].detected_predator) {
				physics->vehicles[i].control_state = DOWN;
			}
			else if (vehicle_sensors[i].ldetected && vehicle_sensors[i].rdetected && !vehicle_sensors[i].detected_predator)
				physics->vehicles[i].control_state = UP;

			else if (vehicle_sensors[i].ldetected && !vehicle_sensors[i].detected_predator)
				physics->vehicles[i].control_state = UP | RIGHT;

			else if (vehicle_sensors[i].rdetected && !vehicle_sensors[i].detected_predator)
				physics->vehicles[i].control_state = UP | LEFT;

			else
				physics->vehicles[i].control_state = 0;

		}
		else {
			if (vehicle_sensors[i].ldetected && vehicle_sensors[i].rdetected)
				physics->vehicles[i].control_state = DOWN;

			else if (vehicle_sensors[i].ldetected)
				physics->vehicles[i].control_state = DOWN | RIGHT;

			else if (vehicle_sensors[i].rdetected)
				physics->vehicles[i].control_state = DOWN | LEFT;

			else
				physics->vehicles[i].control_state = 0;
		}
	}
}