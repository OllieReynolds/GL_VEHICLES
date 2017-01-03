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


void Line_Renderer::draw(const Camera& camera, const vec3& world_space_a, const vec3& world_space_b, const vec4& colour) {
	shader.use();
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	vec3 values[2] = { world_space_a, world_space_b};
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * 2, &values);

	shader.set_uniform("view", camera.matrix_view);
	shader.set_uniform("projection", camera.matrix_projection_persp);
	shader.set_uniform("model", mat4());
	shader.set_uniform("uniform_colour", colour);
	
	glDrawArrays(GL_LINES, 0, 2);
}

void Line_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}