#pragma once

#include <map>
#include <set>

#include <Box2D\Box2D.h>

#include "maths.h"
#include "types.h"
#include "utils.h"

using namespace maths;
using namespace std;

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

static uint16 TYRE    = 0x0001;
static uint16 VEHICLE = 0x0002;
static uint16 ENV	  = 0x0004;


struct VehicleData {
	bool is_predator;
	int instance_id;
};

extern set<pair<VehicleData*, VehicleData*>> vehicle_collision_events;

struct Boundary {
	Boundary(b2World* world, const b2Vec2& position, float angle);

	b2Body* body;
	b2BodyDef body_def;
	b2PolygonShape polygon_shape;

	void destroy();
};

class Tyre {
public:
	Tyre(b2World* world, float max_forward_speed, float max_backward_speed, float max_drive_force, float max_lateral_impulse);

	b2Vec2 get_forward_velocity();
	b2Vec2 get_lateral_velocity();

	void destroy();
	void update_friction();
	void update_drive(float desired_speed);

	float max_forward_speed;
	float max_backward_speed;
	float max_drive_force;
	float max_lateral_impulse;

	b2Body* body;
};


class Vehicle {
public:
	Vehicle();

	void destroy();
	void init(b2World* world, b2Vec2 position, float rotation, bool is_predator, int index);
	void update();

	std::vector<Tyre*> tyres;
	b2RevoluteJoint *fl_joint, *fr_joint;
	b2Body* body;
	float new_angle;
	int index;
	bool is_predator;
	VehicleData* data;

	float desired_angle;
	float desired_speed;
};

class ContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact);
};

class Physics {
public:
	Physics(int num_vehicles, std::map<int, Transform>& transforms, std::map<int, Vehicle_Attributes>& v_attribs);

	void				update();
	void				destroy();
	vec2				get_vehicle_position(int index);
	float				get_vehicle_rotation(int index);
	void				add_vehicle(int instance_id, const Transform& transform, bool is_predator);
	void				remove_vehicle();
	void				remove_vehicle(int index);

	b2Vec2 gravity;
	b2World world;
	Boundary *wall_1, *wall_2, *wall_3, *wall_4;
	map<int, Vehicle> vehicles;
	float time_step;
	int velocity_iterations;
	int position_iterations;


	ContactListener vehicle_contact_listener;
};