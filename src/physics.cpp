#include "..\include\physics.h"

void Physics::update() {
	for (int i = 0; i < num_vehicles; i++) {
		vehicles[i].update();
	}

	world.Step(time_step, velocity_iterations, position_iterations);
}

vec2 Physics::get_vehicle_position(int index) {
	return{ vehicles[index].body->GetPosition().x, vehicles[index].body->GetPosition().y };
}

float Physics::get_vehicle_rotation(int index) {
	return{ vehicles[index].body->GetAngle() * (float)(180 / 3.141592f) };
}

std::vector<vec2> Physics::get_vehicle_positions() {
	std::vector<vec2> positions;

	for (int i = 0; i < num_vehicles; i++) {
		positions.push_back(get_vehicle_position(i));
	}

	return positions;
}

std::vector<float> Physics::get_vehicle_rotations() {
	std::vector<float> rotations;

	for (int i = 0; i < num_vehicles; i++) {
		rotations.push_back(get_vehicle_rotation(i));
	}

	return rotations;
}