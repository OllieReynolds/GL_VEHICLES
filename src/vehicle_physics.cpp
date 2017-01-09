#include "..\include\vehicle_phsyics.h"

void Vehicle_Physics_Controller::update() {
	vehicle->update(vehicle_control_state);
	world.Step(time_step, velocity_iterations, position_iterations);
}

vec2 Vehicle_Physics_Controller::get_vehicle_position() {
	return{ vehicle->body->GetPosition().x, vehicle->body->GetPosition().y };
}

float Vehicle_Physics_Controller::get_vehicle_rotation() {
	return{ vehicle->body->GetAngle() * (float)(180 / 3.141592f) };
}