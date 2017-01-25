#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include <Box2D\Box2D.h>

#include "maths.h"
#include "utils.h"

using namespace maths;
using namespace std;

enum {
	LEFT = 0x1,
	RIGHT = 0X2,
	UP = 0x4,
	DOWN = 0x8
};

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

class Tyre {
public:
	float max_forward_speed;
	float max_backward_speed;
	float max_drive_force;
	float max_lateral_impulse;

	b2Body* body;

	Tyre(b2World* world) {
		b2BodyDef body_def;
		body_def.type = b2_dynamicBody;
		body = world->CreateBody(&body_def);


		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(0.5f, 1.25f);
		body->CreateFixture(&polygonShape, 1.f);

		body->SetUserData(this);
	}

	void destroy() {
		body->GetWorld()->DestroyBody(body);
	}

	void set_characteristics(float max_forward_speed, float max_backward_speed, float max_drive_force, float max_lateral_impulse) {
		this->max_forward_speed = max_forward_speed;
		this->max_backward_speed = max_backward_speed;
		this->max_drive_force = max_drive_force;
		this->max_lateral_impulse = max_lateral_impulse;
	}

	b2Vec2 get_forward_velocity() {
		b2Vec2 forward_normal = body->GetWorldVector(b2Vec2(0, 1));
		return b2Dot(forward_normal, body->GetLinearVelocity()) * forward_normal;
	}

	b2Vec2 get_lateral_velocity() {
		b2Vec2 right_normal = body->GetWorldVector(b2Vec2(1, 0));
		return b2Dot(right_normal, body->GetLinearVelocity()) * right_normal;
	}

	void update_friction() {
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

	void update_drive(int control_state) {
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

	/*void update_turn(int control_state) {
		float desired_torque = 0;

		switch (control_state & (LEFT | RIGHT)) {
			case LEFT: desired_torque = 15; break;
			case RIGHT: desired_torque = 15; break;
		}

		body->ApplyTorque(desired_torque, true);
	}*/
};

class Vehicle {
public:
	Vehicle() { }

	void destroy() {
		for (int i = 0; i < tyres.size(); i++) {
			tyres[i]->destroy();
			delete tyres[i];
		}
	}


	void init(b2World* world, b2Vec2 position, float rotation, int control_state) {
		this->control_state = control_state;
		
		b2BodyDef body_def;
		body_def.type = b2_dynamicBody;
		body_def.position.Set(position.x, position.y);
		//ody_def.angle = rotation;
		body = world->CreateBody(&body_def);
		body->SetAngularDamping(3);

		b2PolygonShape polygon_shape;
		polygon_shape.SetAsBox(8.f, 12.f);
		b2Fixture* fixture = body->CreateFixture(&polygon_shape, 0.1f);

		b2RevoluteJointDef joint_def;
		joint_def.bodyA = body;
		joint_def.enableLimit = true;
		joint_def.lowerAngle = 0;
		joint_def.upperAngle = 0;
		joint_def.localAnchorB.SetZero();

		float max_forward_speed = 250;
		float max_backward_speed = -40;
		float back_tyre_max_drive_force = 300;
		float front_tyre_max_drive_force = 500;
		float back_tyre_max_lateral_impulse = 8.5;
		float front_tyre_max_lateral_impulse = 7.5;

		// Back Left
		Tyre* tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, back_tyre_max_drive_force, back_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(-13.f, -10.75f);
		world->CreateJoint(&joint_def);
		tyres.push_back(tyre);

		// Back Right
		tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, back_tyre_max_drive_force, back_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(13.f, -10.75f);
		world->CreateJoint(&joint_def);
		tyres.push_back(tyre);

		// Front Left
		tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, front_tyre_max_drive_force, front_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(-13.f, 8.5f);
		fl_joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);
		tyres.push_back(tyre);

		// Front Right
		tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, front_tyre_max_drive_force, front_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(13.f, 8.5f);
		fr_joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);
		tyres.push_back(tyre);
	}

	void update() {
		for (int i = 0; i < tyres.size(); i++)
			tyres[i]->update_friction();

		for (int i = 0; i < tyres.size(); i++)
			tyres[i]->update_drive(control_state);


		float lock_angle = 35 * DEGTORAD;
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

	std::vector<Tyre*> tyres;
	b2RevoluteJoint *fl_joint, *fr_joint;
	b2Body* body;
	float new_angle;
	int control_state;
};

class Physics {
public:
	Physics(int num_vehicles, std::vector<utils::Transform>& transforms) : gravity{ 0.f, 0.f }, world(gravity), velocity_iterations(12), position_iterations(12), time_step(1.f / 60.f) {
		vehicles = vector<Vehicle>(num_vehicles);
		for (int i = 0; i < num_vehicles; i++) {
			vehicles[i].init(
				&world, 
				{ transforms[i].position.x, transforms[i].position.z },
				transforms[i].rotation.y, 
				UP
			);
		}

		b2BodyDef body_def;
		b2PolygonShape polygon_shape;

		polygon_shape.SetAsBox(4.f, 800.f);
		body_def.type = b2_staticBody;
		body_def.position = { -390.f, 0.f };
		wall_1 = world.CreateBody(&body_def);
		wall_1->CreateFixture(&polygon_shape, 1.f);

		polygon_shape.SetAsBox(4.f, 800.f);
		body_def.type = b2_staticBody;
		body_def.position = { 390.f, 0.f };
		wall_2 = world.CreateBody(&body_def);
		wall_2->CreateFixture(&polygon_shape, 1.f);

		polygon_shape.SetAsBox(4.f, 800.f);
		body_def.type = b2_staticBody;
		body_def.position = { 0.f, -390.f };
		body_def.angle = to_radians(90.f);
		wall_3 = world.CreateBody(&body_def);
		wall_3->CreateFixture(&polygon_shape, 1.f);

		polygon_shape.SetAsBox(4.f, 800.f);
		body_def.type = b2_staticBody;
		body_def.position = { 0.f, 390.f };
		body_def.angle = to_radians(90.f);
		wall_4 = world.CreateBody(&body_def);
		wall_4->CreateFixture(&polygon_shape, 1.f);

		for (int i = 0; i < vehicles.size(); i++)
			vehicles[i].update();

		world.Step(time_step, velocity_iterations, position_iterations);
		
	}

	/////////////////////////////////////
	// Member Functions
	/////////////////////////////////////
	void				update();
	void				destroy();

	b2AABB				get_vehicle_AABB(int index);

	vec2				get_vehicle_position(int index);
	float				get_vehicle_rotation(int index);
	std::vector<vec2>	get_vehicle_positions();
	std::vector<float>	get_vehicle_rotations();

	void				add_vehicle(const utils::Transform& transform);
	void				remove_vehicle();

	/////////////////////////////////////
	// Member variables
	/////////////////////////////////////
	b2Vec2 gravity;
	b2World world;

	b2Body *wall_1, *wall_2, *wall_3, *wall_4;

	vector<Vehicle> vehicles;

	float time_step;
	int velocity_iterations;
	int position_iterations;
};