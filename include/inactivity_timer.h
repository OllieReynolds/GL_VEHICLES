#pragma once

#include <map>

#include "maths.h"
#include "utils.h"

using namespace maths;
using namespace utils;

using std::map;

struct Inactivity_Timer {
	Inactivity_Timer();

	void init(map<int, Transform> new_transforms);
	void update(map<int, Transform> new_transforms);
	void check_inactivity(map<int, Transform> new_transforms);
	void reset();

	float start_milliseconds;
	float remaining_milliseconds;

	map<int, Transform> old_transforms;
};