#include "..\include\obstacle.h"

namespace simulation {
	void Obstacle::init(const mat4& projection_matrix) {
		shader = {
			"shaders/obstacle.v.glsl",
			"shaders/obstacle.f.glsl"
		};

		shader.set_uniform("projection", projection_matrix);
		//shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));

		vec2 points[4] = {
			{-0.5f, -0.5f},
			{-0.5f,  0.5f},
			{0.5f, -0.5f},
			{0.5f,  0.5f}
		};

		set_gl_buffer_data(sizeof(points), &points);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);	
	}


	void Obstacle::update(const maths::vec2& cursor_pos) {
		position.x = cursor_pos.x;
		position.y = cursor_pos.y;
	}


	void Obstacle::draw(const mat4& view_matrix) {
		
		shader.use();

		shader.set_uniform("model", gen_model_matrix());
		shader.set_uniform("colour", colour);

		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader.release();
		glBindVertexArray(0);
	}

	void Obstacle::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
	}
}