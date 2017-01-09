#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include <Box2D\Box2D.h>

#include "maths.h"

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
		body->CreateFixture(&polygonShape, 1);

		body->SetUserData(this);
	}

	~Tyre() {
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
		default:;
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

	void update_turn(int control_state) {
		float desired_torque = 0;

		switch (control_state & (LEFT | RIGHT)) {
		case LEFT: desired_torque = 15; break;
		case RIGHT: desired_torque = -15; break;
		default:;
		}

		body->ApplyTorque(desired_torque, true);
	}
};

class Vehicle {
public:
	Vehicle(b2World* world) {
		b2BodyDef body_def;
		body_def.type = b2_dynamicBody;
		body = world->CreateBody(&body_def);
		body->SetAngularDamping(3);

		b2Vec2 vertices[8];
		vertices[0].Set(1.5, 0);
		vertices[1].Set(3, 2.5);
		vertices[2].Set(2.8, 5.5);
		vertices[3].Set(1, 10);
		vertices[4].Set(-1, 10);
		vertices[5].Set(-2.8, 5.5);
		vertices[6].Set(-3, 2.5);
		vertices[7].Set(-1.5, 0);
		b2PolygonShape polygon_shape;
		polygon_shape.Set(vertices, 8);
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
		joint_def.localAnchorA.Set(-3.f, 0.75f);
		world->CreateJoint(&joint_def);
		tyres.push_back(tyre);

		// Back Right
		tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, back_tyre_max_drive_force, back_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(3.f, 0.75f);
		world->CreateJoint(&joint_def);
		tyres.push_back(tyre);

		// Front Left
		tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, front_tyre_max_drive_force, front_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(-3.f, 8.5f);
		fl_joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);
		tyres.push_back(tyre);

		// Front Right
		tyre = new Tyre(world);
		tyre->set_characteristics(max_forward_speed, max_backward_speed, front_tyre_max_drive_force, front_tyre_max_lateral_impulse);
		joint_def.bodyB = tyre->body;
		joint_def.localAnchorA.Set(3.f, 8.5f);
		fr_joint = (b2RevoluteJoint*)world->CreateJoint(&joint_def);
		tyres.push_back(tyre);
	}

	~Vehicle() {
		for (int i = 0; i < tyres.size(); i++) delete tyres[i];
	}

	void update(int control_state) {
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
		default:;
		}

		float angle_now = fl_joint->GetJointAngle();
		float angle_to_turn = desired_angle - angle_now;
		angle_to_turn = b2Clamp(angle_to_turn, -turn_per_time_step, turn_per_time_step);
		new_angle = angle_now + angle_to_turn;
		fl_joint->SetLimits(new_angle, new_angle);
		fr_joint->SetLimits(new_angle, new_angle);
	}

	std::vector<Tyre*> tyres;
	b2RevoluteJoint *fl_joint, *fr_joint;
	b2Body* body;
	float new_angle;
};

class Vehicle_Physics_Controller {
public:
	Vehicle_Physics_Controller() : gravity{ 0.f, 0.f }, world(gravity) {
		vehicle_control_state = UP | LEFT;
		vehicle = new Vehicle(&world);

		time_step = 1.f / 60.f;
		velocity_iterations = 3;
		position_iterations = 3;
	}

	~Vehicle_Physics_Controller() {
		delete vehicle;
	}

	void update();
	vec2 get_vehicle_position();
	float get_vehicle_rotation();

	b2Vec2 gravity;
	b2World world;

	int vehicle_control_state;
	Vehicle* vehicle;

	float time_step;
	int velocity_iterations;
	int position_iterations;
};