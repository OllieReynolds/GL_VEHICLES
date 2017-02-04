#include "..\include\simulation.h"


Transform Wheel_Attributes::gen_transform_from_vehicle(const b2Vec2& forward_velocity, const Transform& t, float wheel_dist) {
	float wheel_offset_from_vehicle_angle = t.rotation.y - angular_offset;
	vec2 direction = polar_to_cartesian(to_radians(wheel_offset_from_vehicle_angle)) * wheel_dist;

	Transform transform;
	transform.position = vec3{ t.position.x + direction.x, 4.f, t.position.z + direction.y };
	transform.size = vec3{ 1.f, 1.f, 1.5f };
	transform.rotation = t.rotation;
	transform.rotation.y += y_rotation;

	if (y_rotation == 0.f)
		transform.rotation.z += magnitude(vec2{ forward_velocity.x, forward_velocity.y }) * 3.f;
	else
		transform.rotation.z -= magnitude(vec2{ forward_velocity.x, forward_velocity.y }) * 3.f;

	return transform;
}

Simulation::Simulation() {
	index_state = 1;
	index_selected_vehicle = 0;
	generation = 0;

	mouse_pressed = false;
	is_updating = false;
	is_drawing = true;

	ui = UI(camera);
	text_renderer = Text_Renderer(camera.resolution.x / 60.f, "data/ShareTechMono-Regular.ttf");


	
	// Constructor of Physics makes a ton of objects, but add_vehicle is doing this as well. Works if physics initialised before, but is wrong.
	{
		// Init Physics
		physics = new Physics(transforms_vehicles.size(), transforms_vehicles, attributes_vehicles);
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
	transforms_boundaries[0] = { vec3{ physics->wall_1->GetPosition().x, 10.f, physics->wall_1->GetPosition().y }, vec3{ 4.f, 780.f, 0.f }, vec3{ 90.f, box2d_to_simulation_angle(physics->wall_1->GetAngle()), 0.f } };
	transforms_boundaries[1] = { vec3{ physics->wall_2->GetPosition().x, 10.f, physics->wall_2->GetPosition().y }, vec3{ 4.f, 780.f, 0.f }, vec3{ 90.f, box2d_to_simulation_angle(physics->wall_2->GetAngle()), 0.f } };
	transforms_boundaries[2] = { vec3{ physics->wall_3->GetPosition().x, 10.f, physics->wall_3->GetPosition().y }, vec3{ 4.f, 780.f, 0.f }, vec3{  0.f, box2d_to_simulation_angle(physics->wall_3->GetAngle()), 90.f } };
	transforms_boundaries[3] = { vec3{ physics->wall_4->GetPosition().x, 10.f, physics->wall_4->GetPosition().y }, vec3{ 4.f, 780.f, 0.f }, vec3{  0.f, box2d_to_simulation_angle(physics->wall_4->GetAngle()), 90.f } };

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

	// Remove prey if collided with predator
	//{
	//	vector<int> remove_indices;
	//	for (pair<VehicleData*, VehicleData*> e : vehicle_collision_events) {
	//		if ((e.first->is_predator && !e.second->is_predator) || !e.first->is_predator && e.second->is_predator) {
	//			int index = -1;
	//			if (e.first->is_predator)
	//				remove_indices.push_back( e.second->index);
	//			else
	//				remove_indices.push_back(e.first->index);
	//		}
	//	}

	//	vehicle_collision_events.clear();

	//	for (int i = 0; i < remove_indices.size(); i++) {
	//		remove_vehicle(remove_indices[i]);
	//	}
	//}

	for (int i = 0; i < lights.size(); i++) {
		lights[i].position = transforms_vehicles[i].position;
	}

	if (is_updating) {
		generation = utils::elapsed_time();

		// Update Physics
		physics->update();

		// Vehicles Transforms
		update_simulation_transforms_from_physics();
		update_sensors_from_simulation_transforms();

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
				// Vehicle Sensors
				for (int i = 0; i < vehicle_sensors.size(); i++) {
					tri_renderer.draw_3D_coloured(camera, vehicle_sensors[i].la, vehicle_sensors[i].lb, vehicle_sensors[i].lc, vec4{ 1.f, 1.f, 0.f, 0.01f });
					tri_renderer.draw_3D_coloured(camera, vehicle_sensors[i].ra, vehicle_sensors[i].rb, vehicle_sensors[i].rc, vec4{ 1.f, 1.f, 0.f, 0.01f });							
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
				text_renderer.draw(ui.attributes_ui[i].label, ui.attributes_ui[i].position + vec2{ 0.f, -10.f }, true, colour::white);
			}

			int num_predators = 0;
			int num_prey = 0;

			for (int i = 0; i < attributes_vehicles.size(); i++) {
				if (attributes_vehicles[i].is_predator)
					num_predators++;
				else
					num_prey++;
			}

			string predators = " Predators: " + to_string(num_predators);
			string prey = "      Prey: " + to_string(num_prey);
			string gen = "Generation: " + to_string(generation);

			text_renderer.draw(predators, { 20.f, camera.resolution.y - 100.f }, false, utils::colour::white);
			text_renderer.draw(prey, { 20.f, camera.resolution.y - 130.f }, false, utils::colour::white);
			text_renderer.draw(gen, { 20.f, camera.resolution.y - 160.f }, false, utils::colour::white);
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

	Transform t = {
		vec3{ rand_pos.x, 4.f, rand_pos.y },
		vec3{ 16.f, 2.f, 8.f },
		vec3{ 0.f, utils::gen_random(0.f, 360.f), 0.f }
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

	transforms_wheels.push_back({});
	transforms_wheels.push_back({});
	transforms_wheels.push_back({});
	transforms_wheels.push_back({});

	static const float SENSOR_ANGLE = 70.f;
	static const float SENSOR_OFFSET = 30.f;
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

		vehicle_sensors.push_back(Vehicle_Sensors{ la, lb, lc, ra, rb, rc, {} });
	}

	physics->add_vehicle(t, is_predator);
}

void Simulation::remove_vehicle() {
	if (!transforms_vehicles.empty()) {
		transforms_vehicles.pop_back();
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

void Simulation::remove_vehicle(int index) {
	transforms_vehicles.erase(transforms_vehicles.begin() + index);
	attributes_vehicles.erase(attributes_vehicles.begin() + index);
	
	for (int i = 0; i < 4; i++) {
		transforms_wheels.erase(transforms_wheels.begin() + (index * 4) + i);
	}

	lights.erase(lights.begin() + index);
	vehicle_sensors.erase(vehicle_sensors.begin() + index);

	physics->remove_vehicle(index);

	if (transforms_vehicles.empty())
		camera.follow_vehicle = false;
}

void Simulation::reset() {
	int n = transforms_vehicles.size();
	for (int i = 0; i < n; i++) 
		remove_vehicle();
	for (int i = 0; i < n; i++)
		add_vehicle(gen_random(0.f, 1.f) > 0.5f);
	index_selected_vehicle = 0;
}

void Simulation::check_detected_walls() {
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		

		static const vec2 p0 = { -400.f, -400.f };
		static const vec2 p1 = { -400.f,  400.f };
		static const vec2 p2 = { 400.f,  400.f };
		static const vec2 p3 = { 400.f, -400.f };

		vec2 a = vehicle_sensors[i].la.XZ();
		vec2 b = vehicle_sensors[i].lb.XZ();
		vec2 c = vehicle_sensors[i].lc.XZ();

		if (   utils::shared::Intersecting(p0, p1, a, b, c) 
			|| utils::shared::Intersecting(p1, p2, a, b, c) 
			|| utils::shared::Intersecting(p2, p3, a, b, c) 
			|| utils::shared::Intersecting(p3, p0, a, b, c)) 
		{
			vehicle_sensors[i].detection_events.push_back({0.f, true, false, false, false, true});
		}


		a = vehicle_sensors[i].ra.XZ();
		b = vehicle_sensors[i].rb.XZ();
		c = vehicle_sensors[i].rc.XZ();

		if (   utils::shared::Intersecting(p0, p1, a, b, c)
			|| utils::shared::Intersecting(p1, p2, a, b, c)
			|| utils::shared::Intersecting(p2, p3, a, b, c)
			|| utils::shared::Intersecting(p3, p0, a, b, c))
		{
			vehicle_sensors[i].detection_events.push_back({ 0.f, false, true, false, false, true });
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


		// Wheels
		for (int j = 0; j < 4; j++) {
			transforms_wheels[index] = attributes_wheels[j].gen_transform_from_vehicle(physics->vehicles[i].body->GetLinearVelocity(), transforms_vehicles[i], 8.f);
			index++;
		}
	}
}

void Simulation::update_sensors_from_simulation_transforms() {
	static const float SENSOR_ANGLE = 70.f;
	static const float SENSOR_OFFSET = 30.f;
	static const float SENSOR_RANGE = 400.f;

	int sensor_num = 0;
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		float y = transforms_vehicles[i].position.y - 6.f + (sensor_num++ * .8f);
		float a = transforms_vehicles[i].rotation.y - SENSOR_OFFSET;
		vec2 a_left = polar_to_cartesian(to_radians(a - SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec2 a_right = polar_to_cartesian(to_radians(a + SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vehicle_sensors[i].la = transforms_vehicles[i].position + vec3{ a_left.x, y, a_left.y };
		vehicle_sensors[i].lb = transforms_vehicles[i].position + vec3{ 0.f, y, 0.f };
		vehicle_sensors[i].lc = transforms_vehicles[i].position + vec3{ a_right.x, y, a_right.y };

		y = transforms_vehicles[i].position.y - 6.f + (sensor_num++ * .8f);
		float b = transforms_vehicles[i].rotation.y + SENSOR_OFFSET;
		vec2 b_left = polar_to_cartesian(to_radians(b - SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec2 b_right = polar_to_cartesian(to_radians(b + SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vehicle_sensors[i].ra = transforms_vehicles[i].position + vec3{ b_left.x, y, b_left.y };
		vehicle_sensors[i].rb = transforms_vehicles[i].position + vec3{ 0.f, y, 0.f };
		vehicle_sensors[i].rc = transforms_vehicles[i].position + vec3{ b_right.x, y, b_right.y };
	}
}

void Simulation::check_detected_vehicles() {
	for (int i = 0; i < transforms_vehicles.size(); i++) {
		for (int j = 0; j < transforms_vehicles.size(); j++) {
			if (i != j) {
				vec2 p = transforms_vehicles[j].position.XZ();

				const static float HITBOX_SIZE = 10.f;
				vec2 p1 = p + vec2{ -HITBOX_SIZE, -HITBOX_SIZE };
				vec2 p2 = p + vec2{  HITBOX_SIZE, -HITBOX_SIZE };
				vec2 p3 = p + vec2{ -HITBOX_SIZE,  HITBOX_SIZE };
				vec2 p4 = p + vec2{  HITBOX_SIZE,  HITBOX_SIZE };

				bool ldetected = false;
				bool rdetected = false;

				vec2 a = vehicle_sensors[i].la.XZ();
				vec2 b = vehicle_sensors[i].lb.XZ();
				vec2 c = vehicle_sensors[i].lc.XZ();
				if (point_triangle_intersect(p1, a, b, c) || point_triangle_intersect(p2, a, b, c) ||
					point_triangle_intersect(p3, a, b, c) || point_triangle_intersect(p4, a, b, c))
				{

					if ((attributes_vehicles[i].is_predator && !attributes_vehicles[j].is_predator) || (!attributes_vehicles[i].is_predator && attributes_vehicles[j].is_predator))
						ldetected = true;
				}

				a = vehicle_sensors[i].ra.XZ();
				b = vehicle_sensors[i].rb.XZ();
				c = vehicle_sensors[i].rc.XZ();
				if (point_triangle_intersect(p1, a, b, c) || point_triangle_intersect(p2, a, b, c) ||
					point_triangle_intersect(p3, a, b, c) || point_triangle_intersect(p4, a, b, c))
				{
					if ((attributes_vehicles[i].is_predator && !attributes_vehicles[j].is_predator) || (!attributes_vehicles[i].is_predator && attributes_vehicles[j].is_predator))
						rdetected = true;
				}

				if (ldetected || rdetected) {
					float dist = distance(transforms_vehicles[i].position, transforms_vehicles[j].position);
					vehicle_sensors[i].detection_events.push_back({dist, ldetected, rdetected, attributes_vehicles[j].is_predator, !attributes_vehicles[j].is_predator, false });
				}
			}
		}
	}
}

void Simulation::predator_prey() {
	for (int i = 0; i < transforms_vehicles.size(); i++) {

		if (!vehicle_sensors[i].detection_events.empty()) {

			int index_of_event_with_closest_distance = 0;
			float closest_distance = 10000.f;
			for (int j = 0; j < vehicle_sensors[i].detection_events.size(); j++) {
				if (vehicle_sensors[i].detection_events[j].distance < closest_distance) {
					closest_distance = vehicle_sensors[i].detection_events[j].distance;
					index_of_event_with_closest_distance = j;
				}
			}

			Detection_Event e = vehicle_sensors[i].detection_events[index_of_event_with_closest_distance];

			if (attributes_vehicles[i].is_predator) {
				if (e.detected_prey) {
					if (e.ldetected && e.rdetected)
						physics->vehicles[i].control_state = UP;
					else if (e.ldetected)
						physics->vehicles[i].control_state = UP | RIGHT;
					else if (e.rdetected)
						physics->vehicles[i].control_state = UP | LEFT;
				}
				else {
					physics->vehicles[i].control_state = 0;
				}
			}
			else {
				if (e.detected_predator) {
					if (e.ldetected && e.rdetected)
						physics->vehicles[i].control_state = DOWN;
					else if (e.ldetected)
						physics->vehicles[i].control_state = DOWN | RIGHT;
					else if (e.rdetected)
						physics->vehicles[i].control_state = DOWN | LEFT;
				}
				else {
					physics->vehicles[i].control_state = 0;
				}
			}

			vehicle_sensors[i].detection_events.clear();
		}
		else {
			physics->vehicles[i].control_state = 0;
		}
	}
}