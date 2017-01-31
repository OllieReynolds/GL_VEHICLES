#pragma once

#include "maths.h"
#include "utils.h"

#include <vector>

using namespace maths;
using namespace utils;

struct Camera {
	Camera() { }
	void update(const std::vector<Transform>& transforms, const int target_index);

	bool follow_vehicle;
	bool target_changed;
	int last_target_index;
	float target_distance;
	float field_of_view;
	float aspect_ratio;
	vec2 resolution;
	vec2 depth_range_ortho;
	vec2 depth_range_persp;
	vec3 position_start;
	vec3 position_current;
	vec3 position_target;
	vec3 orientation_up;
	vec3 old_position;
	vec3 old_target;
	mat4 matrix_view;
	mat4 matrix_projection_persp;
	mat4 matrix_projection_ortho;
};