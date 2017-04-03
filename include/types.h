#pragma once

#include "maths.h"
#include <Box2D\Box2D.h>
#include <vector>

namespace {
	using namespace maths;
}

struct Light {
	vec3 position;
	vec3 colour;
	float intensity;
};

struct Transform {
	vec3 position;
	vec3 size;
	vec3 rotation;
};

struct Vehicle_Attributes {
	float forward_speed;
	float turning_speed;
	vec4 colour;
	bool is_predator;
	float energy;
	int id;
};

struct Detection_Event {
	float distance;
	bool ldetected, rdetected;
	bool detected_predator;
	bool detected_prey;
	bool detected_wall;
};

struct Vehicle_Sensors {
	vec3 la, lb, lc, ra, rb, rc;
	float angle, offset, range;
	std::vector<Detection_Event> detection_events;
};

struct Wheel_Attributes {
	Transform gen_transform_from_vehicle(const b2Vec2& forward_velocity, const Transform& t, float wheel_dist);

	float angular_offset;
	float y_rotation;
};