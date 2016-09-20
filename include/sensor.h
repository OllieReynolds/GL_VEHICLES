#pragma once

#include <math.h>

#include "drawable.h"
#include "maths.h"

namespace simulation {
	using namespace maths;
	
	extern const float PI;


	struct Sensor_Attribs {
		Sensor_Attribs();
		Sensor_Attribs(const vec4& colour, const vec2& position, const vec2& heading, float theta, float radius);

		vec4 colour;
		vec2 position;
		vec2 start;
		vec2 end;
		float radius;
	};

	class Sensor : public Drawable {
	public:
		Sensor() {}
		Sensor(const Sensor_Attribs& sa) {
			attribs.colour = sa.colour;
			attribs.end = sa.end;
			attribs.position = sa.position;
			attribs.radius = sa.radius;
			attribs.start = sa.start;
		}

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();


		Sensor_Attribs attribs;

	private:
		static GLuint bind_index;
		
	};
}