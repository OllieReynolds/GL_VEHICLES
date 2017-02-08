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

int Simulation::instance_id = 0;

Simulation::Simulation() {
	index_state = 1;
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
	lights = map<int, Light>();
	for (map<int, Transform>::iterator it = transforms_vehicles.begin(); it != transforms_vehicles.end(); ++it) {
		lights.insert(pair<int, Light>(it->first, { { 0.f, 30.f, 0.f }, attributes_vehicles[it->first].colour.XYZ() }));
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

	{
		vector<int> remove_indices;
		for (pair<VehicleData*, VehicleData*> e : vehicle_collision_events) {
			if ((e.first->is_predator && !e.second->is_predator) || !e.first->is_predator && e.second->is_predator) {
				int index = -1;
				if (e.first->is_predator)
					remove_indices.push_back( e.second->instance_id);
				else
					remove_indices.push_back(e.first->instance_id);
			}
		}

		vehicle_collision_events.clear();

		for (int i = 0; i < remove_indices.size(); i++) {
			remove_vehicle(remove_indices[i]);
			add_vehicle(gen_random(0.f, 1.f) > 0.5f);
		}
	}

	for (map<int, Transform>::iterator it = transforms_vehicles.begin(); it != transforms_vehicles.end(); ++it) {
		lights[it->first].position = it->second.position;
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
	camera.update(transforms_vehicles);

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
				for (map<int, Vehicle_Sensors>::iterator it = vehicle_sensors.begin(); it != vehicle_sensors.end(); ++it) {
					tri_renderer.draw_3D_coloured(camera, it->second.la, it->second.lb, it->second.lc, vec4{ 1.f, 1.f, 0.f, 0.01f });
					tri_renderer.draw_3D_coloured(camera, it->second.ra, it->second.rb, it->second.rc, vec4{ 1.f, 1.f, 0.f, 0.01f });
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

			for (map<int, Vehicle_Attributes>::iterator it = attributes_vehicles.begin(); it != attributes_vehicles.end(); ++it) {
				if (attributes_vehicles[it->first].is_predator)
					num_predators++;
				else
					num_prey++;
			}

			// Side Menu
			{
				float text_x = camera.resolution.x * 0.04685212298f;

				float text_y = camera.resolution.y * 0.85f;
				float text_y_offset = 30.f;

				quad_renderer.draw_2D(camera, { camera.resolution.x * 0.125f, camera.resolution.y / 2.f }, { camera.resolution.x * 0.2f, camera.resolution.y * 0.8f }, { 0.f, 0.f, 0.f, 0.7f });

				string predators =	"Predators:  " + to_string(num_predators);
				string prey =		"Prey:       " + to_string(num_prey);
				string gen =		"Generation: " + to_string(generation);
				text_renderer.draw("SIM INFO",		{ text_x, text_y - (text_y_offset * 0.f) }, false, utils::colour::yellow);
				text_renderer.draw(predators,		{ text_x, text_y - (text_y_offset * 1.f) }, false, utils::colour::white);
				text_renderer.draw(prey,			{ text_x, text_y - (text_y_offset * 2.f) }, false, utils::colour::white);
				text_renderer.draw(gen,				{ text_x, text_y - (text_y_offset * 3.f) }, false, utils::colour::white);

				int y_offset_multiplier = 5;
				text_renderer.draw("ENERGY LEVELS", { text_x, text_y - (text_y_offset * y_offset_multiplier++) }, false, utils::colour::yellow);
				if (attributes_vehicles.empty()) {
					text_renderer.draw("No vehicles running", { text_x, text_y - (text_y_offset * y_offset_multiplier++) }, false, utils::colour::grey);
				} else {
					for (map<int, Vehicle_Attributes>::iterator it = attributes_vehicles.begin(); it != attributes_vehicles.end(); ++it) {
						if (y_offset_multiplier > 17) {
							text_renderer.draw("<more...>", { text_x, text_y - (text_y_offset * y_offset_multiplier++) }, false, utils::colour::white);
							break;
						}
						else {
							string str_i = to_string(it->second.id);
							string str_energy = friendly_float(it->second.energy);
							string display = "Vehicle " + str_i + ":" + str_energy;
							text_renderer.draw(display, { text_x, text_y - (text_y_offset * y_offset_multiplier++) }, false, utils::colour::white);
						}
					}
				}
			}
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


	int key = instance_id++;

	Vehicle_Attributes av = {
		utils::gen_random(0.2f, 0.5f),
		utils::gen_random(2.5f, 3.5f),
		(is_predator) ? utils::colour::red : utils::colour::blue,
		is_predator,
		100.f,
		key
	};

	transforms_vehicles.insert(pair<int, Transform>(key, t));
	attributes_vehicles.insert(pair<int, Vehicle_Attributes>(key, av));
	lights.insert(pair<int, Light>(key, { { 0.f, 30.f, 0.f }, av.colour.XYZ() }));
	transforms_wheels.insert(pair<int, vector<Transform>>(key, { {}, {}, {}, {} }));

	static const float SENSOR_ANGLE = 70.f;
	static const float SENSOR_OFFSET = 30.f;
	static const float SENSOR_RANGE = 400.f;
	{
		float y = t.position.y - 6.f + ((vehicle_sensors.size() + 1) * 0.8f);

		float _a = t.rotation.y - SENSOR_OFFSET;
		vec2 a_left = polar_to_cartesian(to_radians(_a - SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec2 a_right = polar_to_cartesian(to_radians(_a + SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec3 la = t.position + vec3{ a_left.x, y, a_left.y };
		vec3 lb = t.position + vec3{ 0.f, y, 0.f };
		vec3 lc = t.position + vec3{ a_right.x, y, a_right.y };

		y = t.position.y - 6.f + ((vehicle_sensors.size() + 2) * 0.8f);

		float _b = t.rotation.y + SENSOR_OFFSET;
		vec2 b_left = polar_to_cartesian(to_radians(_b - SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec2 b_right = polar_to_cartesian(to_radians(_b + SENSOR_ANGLE / 2.f)) * SENSOR_RANGE;
		vec3 ra = t.position + vec3{ b_left.x, y, b_left.y };
		vec3 rb = t.position + vec3{ 0.f, y, 0.f };
		vec3 rc = t.position + vec3{ b_right.x, y, b_right.y };

		vehicle_sensors.insert(pair<int, Vehicle_Sensors>(key, { la, lb, lc, ra, rb, rc, {} }));
	}

	physics->add_vehicle(key, t, is_predator);
}

void Simulation::remove_vehicle() {
	if (!transforms_vehicles.empty()) {
		transforms_vehicles.erase(--transforms_vehicles.end());
		attributes_vehicles.erase(--attributes_vehicles.end());
		transforms_wheels.erase(--transforms_wheels.end());
		vehicle_sensors.erase(--vehicle_sensors.end());
		lights.erase(--lights.end());
	
		physics->remove_vehicle();

		if (transforms_vehicles.empty()) {
			camera.follow_vehicle = false;
			is_updating = false;
		}
	}
}

void Simulation::remove_vehicle(int instance_id) {
	transforms_vehicles.erase(instance_id);
	attributes_vehicles.erase(instance_id);
	transforms_wheels.erase(instance_id);
	vehicle_sensors.erase(instance_id);
	lights.erase(instance_id);
	
	physics->remove_vehicle(instance_id);

	if (transforms_vehicles.empty())
		camera.follow_vehicle = false;
}

void Simulation::reset() {
	int n = transforms_vehicles.size();
	for (int i = 0; i < n; i++) 
		remove_vehicle();
	for (int i = 0; i < n; i++)
		add_vehicle(gen_random(0.f, 1.f) > 0.5f);
}

void Simulation::check_detected_walls() {
	for (map<int, Transform>::iterator it = transforms_vehicles.begin(); it != transforms_vehicles.end(); ++it) {
		
		int instance_id = it->first;

		static const vec2 p0 = { -400.f, -400.f };
		static const vec2 p1 = { -400.f,  400.f };
		static const vec2 p2 = { 400.f,  400.f };
		static const vec2 p3 = { 400.f, -400.f };

		vec2 a = vehicle_sensors[instance_id].la.XZ();
		vec2 b = vehicle_sensors[instance_id].lb.XZ();
		vec2 c = vehicle_sensors[instance_id].lc.XZ();

		if (   utils::shared::Intersecting(p0, p1, a, b, c) 
			|| utils::shared::Intersecting(p1, p2, a, b, c) 
			|| utils::shared::Intersecting(p2, p3, a, b, c) 
			|| utils::shared::Intersecting(p3, p0, a, b, c)) 
		{
			vehicle_sensors[instance_id].detection_events.push_back({0.f, true, false, false, false, true});
		}


		a = vehicle_sensors[instance_id].ra.XZ();
		b = vehicle_sensors[instance_id].rb.XZ();
		c = vehicle_sensors[instance_id].rc.XZ();

		if (   utils::shared::Intersecting(p0, p1, a, b, c)
			|| utils::shared::Intersecting(p1, p2, a, b, c)
			|| utils::shared::Intersecting(p2, p3, a, b, c)
			|| utils::shared::Intersecting(p3, p0, a, b, c))
		{
			vehicle_sensors[instance_id].detection_events.push_back({ 0.f, false, true, false, false, true });
		}
	}
}

void Simulation::update_simulation_transforms_from_physics() {
	for (map<int, Transform>::iterator it = transforms_vehicles.begin(); it != transforms_vehicles.end(); ++it) {
		int instance_key = it->first;
		it->second.rotation.y = physics->get_vehicle_rotation(instance_key) + 90.f;
		it->second.position = vec3{ physics->get_vehicle_position(instance_key).x, 4.f, physics->get_vehicle_position(instance_key).y };

		for (int i = 0; i < 4; i++) {
			transforms_wheels[instance_key][i] = attributes_wheels[i].gen_transform_from_vehicle(physics->vehicles[instance_key].body->GetLinearVelocity(), it->second, 8.f);
		}
	}
}

void Simulation::update_sensors_from_simulation_transforms() {
	static const float SENSOR_ANGLE = 70.f;
	static const float SENSOR_OFFSET = 30.f;
	static const float SENSOR_RANGE = 400.f;

	int sensor_num = 0;
	for (map<int, Transform>::iterator it = transforms_vehicles.begin(); it != transforms_vehicles.end(); ++it) {
		int instance_key = it->first;

		float y = it->second.position.y - 6.f + (sensor_num++ * .8f);
		float a = it->second.rotation.y - SENSOR_OFFSET;
		vec2 a_left = polar_to_cartesian(to_radians(a - SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec2 a_right = polar_to_cartesian(to_radians(a + SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vehicle_sensors[instance_key].la = it->second.position + vec3{ a_left.x, y, a_left.y };
		vehicle_sensors[instance_key].lb = it->second.position + vec3{ 0.f, y, 0.f };
		vehicle_sensors[instance_key].lc = it->second.position + vec3{ a_right.x, y, a_right.y };

		y = it->second.position.y - 6.f + (sensor_num++ * .8f);
		float b = it->second.rotation.y + SENSOR_OFFSET;
		vec2 b_left = polar_to_cartesian(to_radians(b - SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vec2 b_right = polar_to_cartesian(to_radians(b + SENSOR_ANGLE / 2.F)) * SENSOR_RANGE;
		vehicle_sensors[instance_key].ra = it->second.position + vec3{ b_left.x, y, b_left.y };
		vehicle_sensors[instance_key].rb = it->second.position + vec3{ 0.f, y, 0.f };
		vehicle_sensors[instance_key].rc = it->second.position + vec3{ b_right.x, y, b_right.y };
	}
}

void Simulation::check_detected_vehicles() {
	for (map<int, Transform>::iterator i = transforms_vehicles.begin(); i != transforms_vehicles.end(); ++i) {
		for (map<int, Transform>::iterator j = transforms_vehicles.begin(); j != transforms_vehicles.end(); ++j) {
			int id_i = i->first;
			int id_j = j->first;
			
			if (id_i != id_j) {


				const static float HITBOX_SIZE = 10.f;
				vec2 p = j->second.position.XZ();
				vec2 p1 = p + vec2{ -HITBOX_SIZE, -HITBOX_SIZE };
				vec2 p2 = p + vec2{  HITBOX_SIZE, -HITBOX_SIZE };
				vec2 p3 = p + vec2{ -HITBOX_SIZE,  HITBOX_SIZE };
				vec2 p4 = p + vec2{  HITBOX_SIZE,  HITBOX_SIZE };

				bool ldetected = false;
				bool rdetected = false;

				vec2 a = vehicle_sensors[id_i].la.XZ();
				vec2 b = vehicle_sensors[id_i].lb.XZ();
				vec2 c = vehicle_sensors[id_i].lc.XZ();
				if (point_triangle_intersect(p1, a, b, c) || point_triangle_intersect(p2, a, b, c) ||
					point_triangle_intersect(p3, a, b, c) || point_triangle_intersect(p4, a, b, c))
				{

					if ((attributes_vehicles[id_i].is_predator && !attributes_vehicles[id_j].is_predator) || (!attributes_vehicles[id_i].is_predator && attributes_vehicles[id_j].is_predator))
						ldetected = true;
				}

				a = vehicle_sensors[id_i].ra.XZ();
				b = vehicle_sensors[id_i].rb.XZ();
				c = vehicle_sensors[id_i].rc.XZ();
				if (point_triangle_intersect(p1, a, b, c) || point_triangle_intersect(p2, a, b, c) ||
					point_triangle_intersect(p3, a, b, c) || point_triangle_intersect(p4, a, b, c))
				{
					if ((attributes_vehicles[id_i].is_predator && !attributes_vehicles[id_j].is_predator) || (!attributes_vehicles[id_i].is_predator && attributes_vehicles[id_j].is_predator))
						rdetected = true;
				}

				if (ldetected || rdetected) {
					float dist = distance(i->second.position, j->second.position);
					vehicle_sensors[id_i].detection_events.push_back({dist, ldetected, rdetected, attributes_vehicles[id_j].is_predator, !attributes_vehicles[id_j].is_predator, false });
				}
			}
		}
	}
}

void Simulation::predator_prey() {
	for (map<int, Transform>::iterator it = transforms_vehicles.begin(); it != transforms_vehicles.end(); ++it) {

		int instance_id = it->first;

		if (!vehicle_sensors[instance_id].detection_events.empty()) {

			int index_of_event_with_closest_distance = 0;
			float closest_distance = FLT_MAX;
			for (int j = 0; j < vehicle_sensors[instance_id].detection_events.size(); j++) {
				if (vehicle_sensors[instance_id].detection_events[j].distance < closest_distance) {
					closest_distance = vehicle_sensors[instance_id].detection_events[j].distance;
					index_of_event_with_closest_distance = j;
				}
			}

			Detection_Event e = vehicle_sensors[instance_id].detection_events[index_of_event_with_closest_distance];

			if (attributes_vehicles[instance_id].is_predator) {
				if (e.detected_prey) {
					if (e.ldetected && e.rdetected)  {
						physics->vehicles[instance_id].desired_speed = 100;
					}
					else if (e.ldetected) {
						physics->vehicles[instance_id].desired_speed = 100;
						physics->vehicles[instance_id].desired_angle = -70;
					}
					else if (e.rdetected) {
						physics->vehicles[instance_id].desired_speed = 100;
						physics->vehicles[instance_id].desired_angle = 70;
					}
				}
				else {
					physics->vehicles[instance_id].desired_speed = 0;
					physics->vehicles[instance_id].desired_angle = 0;
				}
			}
			else {
				if (e.detected_predator) {
					if (e.ldetected && e.rdetected) {
						physics->vehicles[instance_id].desired_speed = -100;
					}
					else if (e.ldetected) {
						physics->vehicles[instance_id].desired_speed = -100;
						physics->vehicles[instance_id].desired_angle = 70;
					}
					else if (e.rdetected) {
						physics->vehicles[instance_id].desired_speed = -100;
						physics->vehicles[instance_id].desired_angle = -70;
					}
				}
				else {
					physics->vehicles[instance_id].desired_speed = 0;
					physics->vehicles[instance_id].desired_angle = 0;
				}
			}

			vehicle_sensors[instance_id].detection_events.clear();
		}
		else {
			physics->vehicles[instance_id].desired_speed = 0;
			physics->vehicles[instance_id].desired_angle = 0;
		}
	}
}