#include "..\include\camera.h"
//
//Camera::Camera() {
//
//}

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