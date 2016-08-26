#pragma once

#include <glew.h>

#include "maths.h"
#include "shader.h"
#include "utils.h"

namespace utils {
	class Polygon {
	private:
		GLuint VAO;

	public:
		Polygon(maths::vec2 position = {0.f}, maths::vec2 scale = {512.f}) : position(position), scale(scale) {}

		void init_gl();
		void draw_gl();
		void destroy_gl();

		maths::vec2 scale;
		maths::vec2 position;

		utils::Shader shader;
	};
}