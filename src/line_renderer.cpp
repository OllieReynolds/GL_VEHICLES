#include "../include/line_renderer.h"

void Line_Renderer::init() {
	shader = {
		"shaders/uniform_MVP.v.glsl",
		"shaders/uniform_colour.f.glsl"
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 2, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}


void Line_Renderer::draw(const mat4& view_matrix, const mat4& projection_matrix, const vec3& a, const vec3& b) {
	shader.use();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vec3 values[2] = {a, b};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * 2, &values);

	shader.set_uniform("view", view_matrix);
	shader.set_uniform("projection", projection_matrix);
	shader.set_uniform("model", mat4());

	shader.set_uniform("uniform_colour", colour);
	
	glDrawArrays(GL_LINES, 0, 2);
}

void Line_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}