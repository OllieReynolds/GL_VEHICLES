#include "..\include\camera.h"

Camera::Camera() {
	follow_vehicle = false;
	field_of_view = 90.f;
	target_distance = 50.f;
	resolution = vec2{ 1366.f, 768.f };
	depth_range_ortho = vec2{ -1.f, 1.f };
	depth_range_persp = vec2{ 0.1f, 1500.f };
	position_current = vec3{ 0.f, 0.f, 0.f };
	position_start = vec3{ 0.f, 200.f, 0.f };
	position_target = vec3{ 30.f, 30.f, 30.f };
	orientation_up = vec3{ 0.f, 1.f, 0.f };
	aspect_ratio = resolution.x / resolution.y;
	matrix_view = shared::view_matrix(position_current, position_target, orientation_up);
	matrix_projection_ortho = orthographic_matrix(resolution, depth_range_ortho.x, depth_range_ortho.y, maths::mat4());
	matrix_projection_persp = shared::perspective_matrix(field_of_view, aspect_ratio, depth_range_persp.x, depth_range_persp.y);
}

void Camera::update(const std::vector<Transform>& transforms, const int target_index) {
	if (last_target_index != target_index) {
		target_changed = true;
		old_position = position_current;
		old_target = position_target;
	}

	if (target_changed) {
		static float t = 0.f;

		vec2 direction = polar_to_cartesian(to_radians(transforms[target_index].rotation.y)) * target_distance;

		vec3 p = transforms[target_index].position;
		p.x -= direction.x;
		p.y += target_distance;
		p.z -= direction.y;
		position_current = lerp(old_position, p, t);

		vec3 targ = position_current + vec3{ direction.x, -target_distance, direction.y };
		if (targ.y < -target_distance)
			targ.y = -target_distance;
		position_target = lerp(old_target, targ, t);

		t += 0.01f;

		if (t > 1.f) {
			t = 0.f;
			target_changed = false;
		}
	}

	else if (follow_vehicle) {
		vec2 direction = polar_to_cartesian(to_radians(transforms[target_index].rotation.y));
		direction *= target_distance;

		position_current = transforms[target_index].position;
		position_current.x -= direction.x;
		position_current.y += target_distance;
		position_current.z -= direction.y;
		position_target = position_current + vec3{ direction.x, -target_distance, direction.y };
	}
	else {
		position_current = position_start;
		if (!transforms.empty())
			position_target = transforms[target_index].position;
	}

	matrix_projection_persp = shared::perspective_matrix(field_of_view, aspect_ratio, depth_range_persp.x, depth_range_persp.y);
	matrix_view = shared::view_matrix(position_current, position_target, orientation_up);

	// Must come after the check for current index
	last_target_index = target_index;
}