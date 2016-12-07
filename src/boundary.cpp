#include "..\include\boundary.h"

namespace simulation {
	void Boundary::init(const mat4& projection_matrix) {
		shader = {
			"shaders/boundary.v.glsl",
			"shaders/boundary.f.glsl"
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		vec2 points[4] = {
			{-0.5f, -0.5f},
			{-0.5f,  0.5f},
			{ 0.5f,  0.5f},
		    { 0.5f, -0.5f}
		};

		set_gl_buffer_data(sizeof(points), &points);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	void Boundary::update(const maths::vec2& cursor_pos) {

	}

	void Boundary::draw(const mat4& view_matrix, const mat4& projection_matrix) {
		shader.use();
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		shader.set_uniform("model", gen_model_matrix(size.XY(), position.XY()));

		shader.set_uniform("uniform_colour", colour);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		shader.set_uniform("uniform_colour", vec4{1.f, 1.f, 1.f, 0.3f});
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glBindVertexArray(0);
		shader.release();
	}

	void Boundary::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
	}
}