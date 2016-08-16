#include "..\include\polygon.h"

namespace utils {
	void Polygon::init_gl() {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		shader = {
			"shaders/polygon.v.glsl",
			"shaders/polygon.f.glsl",
			"shaders/polygon.g.glsl"
		};

		glUniform1f(shader.uniform_handle("scale"), scale);
		glUniform2f(shader.uniform_handle("vp"), position[0], position[1]);
		glUniformMatrix4fv(shader.uniform_handle("projection"), 1, GL_FALSE, &utils::resolution_matrix[0][0]);
		glBindVertexArray(0);
	}

	void Polygon::draw_gl() {
		shader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void Polygon::destroy_gl() {
		shader.destroy();
		glDeleteVertexArrays(1, &VAO);
	}
}