#pragma once



#include <glew.h>
#include <glfw3.h>

#include "renderer.h"
#include "maths.h"
#include "model.h"
#include "physics.h"

#include <Windows.h>


using namespace maths;
using namespace utils;

struct Light {
	float ambient;
	vec3 position;
	vec4 colour;
};

struct Vehicle_Sensors {
	vec3 la, lb, lc, ra, rb, rc;
	bool ldetected, rdetected;
};

struct Button_Attributes {
	vec2 position;
	vec2 size;
	vec4 colour;
	std::string label;
};

struct Wheel_Attributes {
	float angular_offset;
	float y_rotation;

	Transform gen_transform_from_vehicle(const Transform& t, float wheel_dist) {
		float wheel_offset_from_vehicle_angle = t.rotation.y - angular_offset;
		vec2 direction = polar_to_cartesian(to_radians(wheel_offset_from_vehicle_angle)) * wheel_dist;

		Transform transform;
		transform.position = vec3{ t.position.x + direction.x, 4.f, t.position.z + direction.y };
		transform.size = vec3{ 1.f, 1.f, 1.f };
		transform.rotation = t.rotation;
		transform.rotation.y += y_rotation;

		if (y_rotation == 0.f)
			transform.rotation.z += 128.f * utils::elapsed_time();
		else 
			transform.rotation.z -= 128.f * utils::elapsed_time();

		return transform;
	}
};

struct UI {
	UI() { }
	UI(const Camera& camera) : index_active_button(-1), index_pressed_button(-1) {
		attributes_ui = vector<Button_Attributes>();
		std::string button_labels[7] = { "ADD", "REMOVE", "EDIT", "FOLLOW", "PLAY", "PAUSE", "RESTARTAAAAAAAAAAAAAA" };
		for (int i = 0; i < 7; i++) {
			float width_by_buttons = camera.resolution.x / 7;
			float p = (i * width_by_buttons) + (width_by_buttons * 0.5f);
			attributes_ui.push_back( { { p, 740.f },{ 206.f, 32.f }, utils::colour::black, button_labels[i] });
		}
	}

	void update(const vec2& cursor_position, const bool mouse_pressed) {
		index_active_button = -1;
		index_pressed_button = -1;

		for (int i = 0; i < attributes_ui.size(); i++) {
			float l = attributes_ui[i].position.x - (attributes_ui[i].size.x * 0.5f);
			float r = attributes_ui[i].position.x + (attributes_ui[i].size.x * 0.5f);
			float u = attributes_ui[i].position.y + (attributes_ui[i].size.y * 0.5f);
			float d = attributes_ui[i].position.y - (attributes_ui[i].size.y * 0.5f);
			if (utils::point_quad_intersect(cursor_position, l, r, u, d)) index_active_button = i;
		}

		if (mouse_pressed && index_active_button != -1) 
			index_pressed_button = index_active_button;
	}

	int index_active_button;
	int index_pressed_button;
	vector<Button_Attributes> attributes_ui;
};

enum SIMULATION_STATE {
	RUNNING = 0,
	PAUSED = 1
};

class Simulation {
public:
	Simulation();

	void init();
	void update();
	void draw();
	void destroy();

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

	Light* lights;
	UI ui;

	bool mouse_pressed;
	
	bool is_updating;
	bool is_drawing;

	int index_selected_vehicle;
	int index_state;
	int num_lights;

	vec2 cursor_position;

	Camera camera;
	Physics* physics;

	vector<Vehicle_Attributes>	attributes_vehicles;
	vector<Wheel_Attributes>	attributes_wheels;

	vector<Vehicle_Sensors>		vehicle_sensors;

	vector<Transform>			transforms_vehicles;
	vector<Transform>			transforms_wheels;
	vector<Transform>			transforms_walls;
	vector<Transform>			transforms_boundaries;
	vector<Transform>			transforms_shadows;
};