#pragma once

#include <glew.h>
#include <glfw3.h>

#include "camera.h"
#include "maths.h"
#include "model.h"
#include "physics.h"
#include "renderer.h"
#include "ui.h"

using namespace maths;
using namespace utils;

struct Vehicle_Sensors {
	vec3 la, lb, lc, ra, rb, rc;
	bool ldetected, rdetected;
	bool detected_predator;
};

struct Wheel_Attributes {
	float angular_offset;
	float y_rotation;

	Transform gen_transform_from_vehicle(const b2Vec2& forward_velocity, const Transform& t, float wheel_dist) {
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
};

class Simulation {
public:
	Simulation();

	void init();
	void update();
	void draw();
	void destroy();

	void update_simulation_transforms_from_physics();
	void update_sensors_from_simulation_transforms();
	void check_detected_walls();
	void check_detected_vehicles();
	void predator_prey();

	void add_vehicle(bool is_predator);
	void remove_vehicle();
	void reset();


	Cube_Renderer cube_renderer;
	Line_Renderer line_renderer;
	Quad_Renderer quad_renderer;
	Text_Renderer text_renderer;
	Model_Renderer model_renderer;
	Circle_Renderer circle_renderer;
	Triangle_Renderer tri_renderer;

	Model wheel_model;
	Model grid_model;

	Texture wheel_texture;
	Texture floor_texture;
	Texture shadow_texture;

	UI ui;
	Camera camera;
	Physics* physics;

	bool mouse_pressed;
	bool is_updating;
	bool is_drawing;
	int index_selected_vehicle;
	int index_state;

	vec2 cursor_position;

	// Environment Properties
	vector<Light>				lights;
	vector<Transform>			transforms_walls;
	vector<Transform>			transforms_boundaries;
	
	// Vehicle Properties
	vector<Vehicle_Attributes>	attributes_vehicles;
	vector<Wheel_Attributes>	attributes_wheels;
	vector<Vehicle_Sensors>		vehicle_sensors;
	vector<Transform>			transforms_vehicles;
	vector<Transform>			transforms_wheels;
	vector<Transform>			transforms_shadows;
};