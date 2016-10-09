#include "..\include\obstacle.h"

namespace simulation {
	Obstacle::Obstacle(const vec4& colour, const vec2& position, const vec2& size, const float rotation) : 
		colour(colour), position(position), size(size), rotation(rotation)
	{

	}


	void Obstacle::init() {
		shader = {
			"shaders/obstacle.v.glsl",
			"shaders/obstacle.f.glsl"
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		vec2 points[4] = {
			{-0.5f, -0.5f},
			{-0.5f,  0.5f},
			{0.5f, -0.5f},
			{0.5f,  0.5f}
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 4, &points, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);	
	}


	void Obstacle::update(const maths::vec2& cursor_pos) {
		move(cursor_pos);
	}


	void Obstacle::draw() {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);
		shader.use();

		mat4 s = scale({size.x, size.y, 0.f});
		mat4 t = transpose(translate({position.x, position.y, 0.f}));
		mat4 r = rotate(rotation);
		mat4 m = mult(mult(s, r), t);

		shader.set_uniform("model", m);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader.release();
		glBindVertexArray(0);
	}

	void Obstacle::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		shader.destroy();
		glDeleteVertexArrays(1, &gl_array_object);
	}


	void Obstacle::move(const vec2& cursor_position) {
		position = cursor_position;
	}
}