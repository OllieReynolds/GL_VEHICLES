#include "..\include\cube_renderer.h"

void Cube_Renderer::init() {
	shader = {
		"shaders/MVP_NORMALS.v.glsl",
		"shaders/DIFFUSE.f.glsl",
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(utils::data::mesh::cube_vertices_normals), &utils::data::mesh::cube_vertices_normals, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Cube_Renderer::draw(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, float rotation, const vec4& colour) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader.use();

	shader.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
	shader.set_uniform("view", view_matrix);
	shader.set_uniform("projection", projection_matrix);
	shader.set_uniform("uniform_colour", colour);
	shader.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, 36);

	shader.release();
	glBindVertexArray(0);
}

void Cube_Renderer::draw_multiple(int n, const mat4& view_matrix, const mat4& projection_matrix, const Transform* transforms, const vec4& colour) {
	shader.use();

	shader.set_uniform("view", view_matrix);
	shader.set_uniform("projection", projection_matrix);
	shader.set_uniform("uniform_colour", colour);
	shader.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });

	for (int i = 0; i < n; i++) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		shader.set_uniform("model", utils::gen_model_matrix(transforms[i].size, transforms[i].position, transforms[i].rotation));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	shader.release();
}

void Cube_Renderer::destroy() {
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	shader.destroy();
}