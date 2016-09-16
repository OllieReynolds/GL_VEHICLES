#pragma once

#include <math.h>

#include "maths.h"

namespace simulation {
	using namespace maths;
	
	extern const float PI;

	struct Sensor {
		vec4 colour;
		vec2 position;
		vec2 start;
		vec2 end;
		float radius;

		Sensor(const vec4& colour = vec4{1.f}, const vec2& position = vec2{1.f}, const vec2& heading = vec2{1.f}, 
			const float theta = 1.f, const float radius = 1.f);

		void set_sector_arms(const vec2& heading, float theta);
	};
}