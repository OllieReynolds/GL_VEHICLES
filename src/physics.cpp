#include "..\include\physics.h"

void Physics::update() {
	world.Step(time_step, velocity_iterations, position_iterations);

	for (int i = 0; i < vehicles.size(); i++)
		vehicles[i].update();
}

b2AABB Physics::get_vehicle_AABB(int index) {
	b2AABB aabb;
	aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);
	b2Fixture* fixture = vehicles[index].body->GetFixtureList();

	while (fixture != NULL) {
		aabb.Combine(aabb, fixture->GetAABB(0));
		fixture = fixture->GetNext();
	}

	return aabb;
}

vec2 Physics::get_vehicle_position(int index) {
	return{ vehicles[index].body->GetPosition().x, vehicles[index].body->GetPosition().y };
}

float Physics::get_vehicle_rotation(int index) {
	return{ vehicles[index].body->GetAngle() * (float)(180 / 3.141592f) };
}

std::vector<vec2> Physics::get_vehicle_positions() {
	std::vector<vec2> positions;
	for (int i = 0; i < vehicles.size(); i++)
		positions.push_back(get_vehicle_position(i));
	return positions;
}

std::vector<float> Physics::get_vehicle_rotations() {
	std::vector<float> rotations;
	for (int i = 0; i < vehicles.size(); i++)
		rotations.push_back(get_vehicle_rotation(i));
	return rotations;
}

void Physics::destroy() {
	for (int i = 0; i < vehicles.size(); i++)
		vehicles[i].destroy();
}

void Physics::add_vehicle(const utils::Transform& transform) {
	b2Vec2 position = { transform.position.x, transform.position.z };
	Vehicle v;
	v.init(&world, position, transform.rotation.y - 90.f, UP | LEFT);
	vehicles.push_back(v);
}

void Physics::remove_vehicle() {
	vehicles.back().destroy();
	vehicles.pop_back();
}