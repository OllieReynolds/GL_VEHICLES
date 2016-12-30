#include "..\include\circle_renderer.h"

void Circle_Renderer::init() {
	shader_2D = {
		"shaders/uniform_MP.v.glsl",
		"shaders/uniform_colour_circle.f.glsl"
	};

	shader_3D = {
		"shaders/uniform_MVP.v.glsl",
		"shaders/uniform_colour_circle.f.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data::mesh::quad_points_textured), &data::mesh::quad_points_textured, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Circle_Renderer::draw_2D(const mat4& view_matrix, const mat4& projection_matrix, const vec2& position, const vec2& size, const vec4& colour, bool filled) {
	shader_2D.use();
	glBindVertexArray(vao);

	shader_2D.set_uniform("uniform_colour", colour);
	shader_2D.set_uniform("projection", projection_matrix);
	shader_2D.set_uniform("model", utils::gen_model_matrix(size, position));
	shader_2D.set_uniform("draw_filled", filled);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_2D.release();
}

void Circle_Renderer::draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour, bool filled) {
	shader_3D.use();
	glBindVertexArray(vao);

	shader_3D.set_uniform("uniform_colour", colour);
	shader_3D.set_uniform("projection", projection_matrix);
	shader_3D.set_uniform("view", view_matrix);
	shader_3D.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
	shader_2D.set_uniform("draw_filled", filled);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);
	shader_3D.release();
}

void Circle_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader_2D.destroy();
}