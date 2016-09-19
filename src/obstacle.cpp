#include "..\include\obstacle.h"

namespace simulation {
	void Obstacle::init() {
		shader = {
			"shaders/obstacle.v.glsl",
			"shaders/obstacle.f.glsl",
			"shaders/obstacle.g.glsl"
		};

		shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		glGenVertexArrays(1, &gl_array_object);
		glBindVertexArray(gl_array_object);

		glGenBuffers(1, &gl_buffer_object);
		glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer_object);

		glBufferData(GL_UNIFORM_BUFFER, sizeof(Obstacle_Attribs), NULL, GL_STATIC_DRAW);
		GLuint bind_index = 1;
		glBindBufferBase(GL_UNIFORM_BUFFER, bind_index, gl_buffer_object);
		glUniformBlockBinding(shader.program, glGetUniformBlockIndex(shader.program, "Obstacles"), bind_index);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindVertexArray(0);
		shader.release();
	
	}


	void Obstacle::update(const maths::vec2& cursor_pos) {
		move(cursor_pos);
	}


	void Obstacle::draw() {
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_UNIFORM_BUFFER, gl_buffer_object);
		shader.use();

		glBufferData(GL_UNIFORM_BUFFER, sizeof(Obstacle_Attribs), &attribs, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, 1);

		shader.release();
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindVertexArray(0);
	

	}

	void Obstacle::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		shader.destroy();
		glDeleteVertexArrays(1, &gl_array_object);
	}


	void Obstacle::move(const vec2& cursor_position) {
		attribs.position = cursor_position;
	}
}