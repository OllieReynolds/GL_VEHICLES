#pragma once

#include <glew.h>

#include "maths.h"
#include "shader.h"
#include "utils.h"

namespace utils {
	class Polygon {
	public:
		Polygon(maths::vec2 position = {16.f, 16.f}, float scale = 32.f) : position(position), scale(scale) {}

		void init_gl();
		void draw_gl();
		void destroy_gl();

		inline void set_position(const maths::vec2& p) { position = p; }

	private:
		GLuint VAO;

		float scale;
		maths::vec2 position;
		utils::Shader shader;
	};
}