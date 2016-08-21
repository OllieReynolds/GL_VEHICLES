#include "..\include\polygon.h"

namespace utils {
	void Polygon::init_gl() {
		shader = {
			"shaders/polygon.v.glsl",
			"shaders/polygon.f.glsl",
			"shaders/polygon.g.glsl"
		};

		shader.set_uniform("scale", scale);
		shader.set_uniform("vp", position);
		shader.set_uniform("colour", maths::vec4{1.f, 0.f, 0.f, 1.f});
		shader.set_uniform("projection", utils::resolution_matrix);
	
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindVertexArray(0);
	}

	void Polygon::draw_gl() {
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void Polygon::destroy_gl() {
		shader.destroy();
		glDeleteVertexArrays(1, &VAO);
	}
}