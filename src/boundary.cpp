#include "..\include\boundary.h"

namespace simulation {

	Boundary::Boundary() :
	    position(0.f), size(0.f) { }

	Boundary::Boundary(const vec2& pos, const vec2& sz) :
		position(pos), size(sz) { }

	void Boundary::init() {
		shader = {
			"shaders/boundary.v.glsl",
			"shaders/boundary.f.glsl"
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 1000.f}, -1.f, 1.f, maths::mat4()));

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		vec2 points[4] = {
			{-0.5f,  0.5f},
			{-0.5f, -0.5f},
			{ 0.5f, -0.5f},
			{ 0.5f,  0.5f}
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

		mat4 s = scale({size.x, size.y, 0.f});
		mat4 t = transpose(translate({position.x, position.y, 0.f}));
		mat4 m = mult(s, t);
		shader.set_uniform("model", m);

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