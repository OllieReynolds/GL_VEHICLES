#include "..\include\cube.h"

namespace simulation {

	void Cube::init(const mat4& projection_matrix) {
		shader = {
			"shaders/cube.v.glsl",
			"shaders/cube.f.glsl"
		};

		shader.set_uniform("projection", projection_matrix);
		//shader.set_uniform("projection", maths::orthographic_matrix({1366.f, 768.f}, -1.f, 1.f, maths::mat4()));
		//shader.set_uniform("projection", shared::perspective_matrix(90.f, 1.7786f, 0.1f, 250.f));
		
		//shader.set_uniform("projection", shared::perspective(180.f, 1366.f, 768.f, 0.1f, 100.f));


		float points[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		set_gl_buffer_data(sizeof(points), &points);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}

	void Cube::update(const maths::vec2& cursor_pos) {
		//rotation = sin(utils::elapsed_time()) * 180.f;
	}

	void Cube::draw(const mat4& view_matrix) {
		shader.use();
		glBindVertexArray(gl_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);


		shader.set_uniform("view", shared::view_matrix(
		    {0.f, 5.f, -5.f}, 
		    {0.f, 0.f, 2.f}, 
		    {0.f, 1.f, 0.f}
		));

		shader.set_uniform("model", gen_model_matrix());

		shader.set_uniform("uniform_colour", maths::vec4(1.f, 0.f, 1.f, 1.f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
		glLineWidth(2.f);
		shader.set_uniform("uniform_colour", maths::vec4(0.f, 1.f, 1.f, 1.f));
		glDrawArrays(GL_LINE_STRIP, 0, 36);
	}

	void Cube::destroy() {
		glDeleteBuffers(1, &gl_buffer_object);
		glDeleteVertexArrays(1, &gl_array_object);
		shader.destroy();
	}
}

