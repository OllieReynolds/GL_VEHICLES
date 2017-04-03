#include "..\include\types.h"

Transform Wheel_Attributes::gen_transform_from_vehicle(const b2Vec2& forward_velocity, const Transform& t, float wheel_dist) {
	float wheel_offset_from_vehicle_angle = t.rotation.y - angular_offset;
	vec2 direction = polar_to_cartesian(to_radians(wheel_offset_from_vehicle_angle)) * wheel_dist;

	Transform transform;
	transform.position = vec3{ t.position.x + direction.x, 4.f, t.position.z + direction.y };
	transform.size = vec3{ 1.f, 1.f, 1.5f };
	transform.rotation = t.rotation;
	transform.rotation.y += y_rotation;

	transform.rotation.z += 0.5f;

	if (y_rotation == 0.f)
		transform.rotation.z += magnitude(vec2{ forward_velocity.x, forward_velocity.y }) * 3.f;
	else
		transform.rotation.z -= magnitude(vec2{ forward_velocity.x, forward_velocity.y }) * 3.f;

	return transform;
}