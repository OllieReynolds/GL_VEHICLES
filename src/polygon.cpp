#include "..\include\polygon.h"

namespace utils {
	void Polygon::init_gl() {
		shader = {
			"shaders/polygon.v.glsl",
			"shaders/polygon.f.glsl",
			"shaders/polygon.g.glsl"
		};

		glUniform1f(shader.uniform_handle("scale"), scale);
		glUniform2fv(shader.uniform_handle("vp"), 1, &position[0]);
		glUniformMatrix4fv(shader.uniform_handle("projection"), 1, GL_FALSE, &utils::resolution_matrix[0][0]);
	
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindVertexArray(0);
	}

	void Polygon::draw_gl() {
		shader.use();
		glUniform2fv(shader.uniform_handle("vp"), 1, &position[0]);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 1);
		glBindVertexArray(0);
	}

	void Polygon::destroy_gl() {
		shader.destroy();
		glDeleteVertexArrays(1, &VAO);
	}
}