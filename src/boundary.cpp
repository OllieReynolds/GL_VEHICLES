#include "..\include\boundary.h"

namespace simulation {
	void Boundary::init() {
		shader = {
			"shaders/boundary.v.glsl",
			"shaders/boundary.f.glsl"
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		vec2 points[4] = {
			{-0.5f, -0.5f},
			{-0.5f,  0.5f},
			{ 0.5f,  0.5f},
		    { 0.5f, -0.5f}
		};

		

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, &points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	void Boundary::update(const maths::vec2& cursor_pos) {

	}

	void Boundary::draw() {
		shader.use();
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		mat4 s = scale({transform.size, 0.f});
		mat4 t = transpose(translate({transform.position, 0.f}));
		mat4 m = mult(s, t);
		shader.set_uniform("model", m);

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