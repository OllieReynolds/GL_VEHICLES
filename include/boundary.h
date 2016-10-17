#pragma once

#include "maths.h"
#include "drawable.h"

namespace simulation {

	using namespace maths;

	class Boundary : public Drawable {
	public:
		Boundary();
		Boundary(const vec2& pos, const vec2& sz);

		void init();
		void update(const maths::vec2& cursor_pos);
		void draw();
		void destroy();

		

	private:
		vec2 position;
		vec2 size;
		
	};
}