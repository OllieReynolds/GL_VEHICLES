#include "..\include\physics.h"

set<pair<VehicleData*, VehicleData*>> vehicle_collision_events;

Tyre::Tyre(b2World* world, float max_forward_speed, float max_backward_speed, float max_drive_force, float max_lateral_impulse) 
	: max_forward_speed(max_forward_speed), max_backward_speed(max_backward_speed), max_drive_force(max_drive_force), max_lateral_impulse(max_lateral_impulse) 
{
	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body = world->CreateBody(&body_def);

	b2PolygonShape polygonShape;
	polygonShape.SetAsBox(0.4, 0.4);
	b2Fixture* f = body->CreateFixture(&polygonShape, 1.f);
	
	
	b2Filter filter;
	filter.categoryBits = TYRE;
	filter.maskBits = ENV;
	filter.groupIndex = 0;

	f->SetFilterData(filter);

	body->SetUserData(this);
}

void Tyre::destroy() {
	body->GetWorld()->DestroyBody(body);
}

b2Vec2 Tyre::get_forward_velocity() {
	b2Vec2 forward_normal = body->GetWorldVector(b2Vec2(0, 1));
	return b2Dot(forward_normal, body->GetLinearVelocity()) * forward_normal;
}

b2Vec2 Tyre::get_lateral_velocity() {
	b2Vec2 right_normal = body->GetWorldVector(b2Vec2(1, 0));
	return b2Dot(right_normal, body->GetLinearVelocity()) * right_normal;
}

void Tyre::update_friction() {
	b2Vec2 impulse = body->GetMass() * -get_lateral_velocity();
	if (impulse.Length() > max_lateral_impulse) {
		impulse *= max_lateral_impulse / impulse.Length();
	}
	body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
	body->ApplyAngularImpulse(.1f * body->GetInertia() * -body->GetAngularVelocity(), true);

	b2Vec2 forward_normal = get_forward_velocity();
	float forward_speed = forward_normal.Normalize();
	float drag_magnitude = -2 * forward_speed;
	body->ApplyForce(drag_magnitude * forward_normal, body->GetWorldCenter(), true);
}

void Tyre::update_drive(int control_state) {
	float desired_speed = 0;

	switch (control_state & (UP | DOWN)) {
		case UP: desired_speed = max_forward_speed; break;
		case DOWN: desired_speed = max_backward_speed; break;
	}

	b2Vec2 forward_normal = body->GetWorldVector(b2Vec2(0, 1));
	float current_speed = b2Dot(get_forward_velocity(), forward_normal);

	float force = 0;

	if (desired_speed > current_speed)
		force = max_drive_force;
	else if (desired_speed < current_speed)
		force = -max_drive_force;
	else
		return;

	body->ApplyForce(force * forward_normal, body->GetWorldCenter(), true);
}

Vehicle::Vehicle() {

}

void Vehicle::destroy() {
	for (int i = 0; i < tyres.size(); i++) {
		tyres[i]->destroy();
		delete tyres[i];
	}

	body->GetWorld()->DestroyBody(body);
	delete data;
}


void Vehicle::init(b2World* world, b2Vec2 position, float rotation, int control_state, bool is_predator, int index) {
	this->control_state = control_state;
	this->is_predator = is_predator;

	b2BodyDef body_def;
	body_def.type = b2_dynamicBody;
	body_def.position.Set(position.x, position.y);
	body = world->CreateBody(&body_def);
	body->SetAngularDamping(2);

	b2PolygonShape polygon_shape;
	polygon_shape.SetAsBox(6.f, 10.f);
	b2Fixture* fixture = body->CreateFixture(&polygon_shape, 0.1f);

	data = new VehicleData;
	data->index = index;
	data->is_predator = is_predator;
	fixture->SetUserData((VehicleData*)data);

	b2Filter filter;
	filter.categoryBits = VEHICLE;
	filter.maskBits = VEHICLE;
	filter.groupIndex = 0;
	fixture->SetFilterData(filter);

	b2RevoluteJointDef joint_def;
	joint_def.bodyA = body;
	joint_def.enableLimit = true;
	joint_def.lowerAngle = 0;
	joint_def.upperAngle = 0;
	joint_def.localAnchorB.SetZero();

	float max_forward_speed = 100;
	float max_backward_speed = -100;
	float back_tyre_max_drive_force = 300;
	float front_tyre_max_drive_force = 100;
	float back_tyre_max_lateral_impulse = 32.f;
	float front_tyre_max_lateral_impulse = 32.f;

	// Back Left
	Tyre* tyre = new Tyre(world, max_forward_speed, max_backward_speed, back_tyre_max_drive_force, back_tyre_max_lateral_impulse);
	joint_def.bodyB = tyre->body;
	joint_def.localAnchorA.Set(-13.f, 0.75f);
	world->CreateJoint(&joint_def);
	tyres.push_back(tyre);

	// Back Right
	tyre = new Tyre(world, max_forward_speed, max_backward_speed, back_tyre_max_drive_force, back_tyre_max_lateral_impulse);
	joint_def.bodyB = tyre->body;
	joint_def.localAnchorA.Set(13.f, 0.75f);
	world->CreateJoint(&joint_def);
	tyres.push_back(tyre);

	// Front Left
	tyre = new Tyre(world, max_forward_speed, max_backward_speed, front_tyre_max_drive_force, front_tyre_max_lateral_impulse);
	joint_def.bodyB = tyre->body;
	joint_def.localAnchorA.Set(-13.f, 8.5f);
	fl_joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);
	tyres.push_back(tyre);

	// Front Right
	tyre = new Tyre(world, max_forward_speed, max_backward_speed, front_tyre_max_drive_force, front_tyre_max_lateral_impulse);
	joint_def.bodyB = tyre->body;
	joint_def.localAnchorA.Set(13.f, 8.5f);
	fr_joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);
	tyres.push_back(tyre);
}

void Vehicle::update() {
	for (int i = 0; i < tyres.size(); i++)
		tyres[i]->update_friction();

	for (int i = 0; i < tyres.size(); i++)
		tyres[i]->update_drive(control_state);


	float lock_angle = 70 * DEGTORAD;
	float turn_speed_per_second = 160 * DEGTORAD;
	float turn_per_time_step = turn_speed_per_second / 60.f;
	float desired_angle = 0;

	switch (control_state & (LEFT | RIGHT)) {
		case LEFT: desired_angle = lock_angle;  break;
		case RIGHT: desired_angle = -lock_angle;  break;
	}

	float angle_to_turn = desired_angle - fl_joint->GetJointAngle();
	angle_to_turn = b2Clamp(angle_to_turn, -turn_per_time_step, turn_per_time_step);
	new_angle = fl_joint->GetJointAngle() + angle_to_turn;
	fl_joint->SetLimits(new_angle, new_angle);
	fr_joint->SetLimits(new_angle, new_angle);
}

void ContactListener::BeginContact(b2Contact* contact) {

	b2Filter fA = contact->GetFixtureA()->GetFilterData();
	b2Filter fB = contact->GetFixtureB()->GetFilterData();

	if (fA.categoryBits == VEHICLE && fB.categoryBits == VEHICLE) {

		VehicleData* dA = (VehicleData*)contact->GetFixtureA()->GetUserData(); 
		VehicleData* dB = (VehicleData*)contact->GetFixtureB()->GetUserData();

		vehicle_collision_events.insert(pair<VehicleData*, VehicleData*>(dA, dB));
	}
		
}

Physics::Physics(int num_vehicles, std::vector<utils::Transform>& transforms, std::vector<utils::Vehicle_Attributes>& v_attribs)
	: gravity{ 0.f, 0.f }, world(gravity), velocity_iterations(12), position_iterations(12), time_step(1.f / 30.f) 
{
	vehicles = vector<Vehicle>(num_vehicles);
	for (int i = 0; i < num_vehicles; i++) {
		vehicles[i].init(
			&world,
			{ transforms[i].position.x, transforms[i].position.z },
			transforms[i].rotation.y,
			0,
			v_attribs[i].is_predator,
			i
		);
	}

	b2BodyDef body_def;
	b2PolygonShape polygon_shape;

	polygon_shape.SetAsBox(4.f, 800.f);
	body_def.type = b2_staticBody;
	body_def.position = { -390.f, 0.f };
	wall_1 = world.CreateBody(&body_def);
	b2Fixture* f = wall_1->CreateFixture(&polygon_shape, 1.f);
	b2Filter filter;
	filter.categoryBits = ENV;
	filter.maskBits = TYRE | VEHICLE;
	filter.groupIndex = 0;
	f->SetFilterData(filter);

	polygon_shape.SetAsBox(4.f, 800.f);
	body_def.type = b2_staticBody;
	body_def.position = { 390.f, 0.f };
	wall_2 = world.CreateBody(&body_def);
	f = wall_2->CreateFixture(&polygon_shape, 1.f);
	filter.categoryBits = ENV;
	filter.maskBits = TYRE | VEHICLE;
	filter.groupIndex = 0;
	f->SetFilterData(filter);

	polygon_shape.SetAsBox(4.f, 800.f);
	body_def.type = b2_staticBody;
	body_def.position = { 0.f, -390.f };
	body_def.angle = to_radians(90.f);
	wall_3 = world.CreateBody(&body_def);
	f = wall_3->CreateFixture(&polygon_shape, 1.f);
	filter.categoryBits = ENV;
	filter.maskBits = TYRE | VEHICLE;
	filter.groupIndex = 0;
	f->SetFilterData(filter);

	polygon_shape.SetAsBox(4.f, 800.f);
	body_def.type = b2_staticBody;
	body_def.position = { 0.f, 390.f };
	body_def.angle = to_radians(90.f);
	wall_4 = world.CreateBody(&body_def);
	f = wall_4->CreateFixture(&polygon_shape, 1.f);
	filter.categoryBits = ENV;
	filter.maskBits = TYRE | VEHICLE;
	filter.groupIndex = 0;
	f->SetFilterData(filter);

	world.SetContactListener(&vehicle_contact_listener);

	for (int i = 0; i < vehicles.size(); i++)
		vehicles[i].update();

	world.Step(time_step, velocity_iterations, position_iterations);
}

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

void Physics::add_vehicle(const utils::Transform& transform, bool is_predator) {
	b2Vec2 position = { transform.position.x, transform.position.z };
	Vehicle v;
	v.init(&world, position, transform.rotation.y, 0, is_predator, vehicles.size() + 1);
	vehicles.push_back(v);
}

void Physics::remove_vehicle() {
	vehicles.back().destroy();
	vehicles.pop_back();
}

void Physics::remove_vehicle(int index) {
	vehicles[index].destroy();
	vehicles.erase(vehicles.begin() + index);
}