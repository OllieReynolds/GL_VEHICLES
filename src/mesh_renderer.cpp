#include "..\include\mesh_renderer.h"

void Mesh_Renderer::init(Mesh& mesh) {
	shader_coloured = {
		"shaders/MVP_NORMALS.v.glsl",
		"shaders/DIFFUSE.f.glsl",
	};
	
	shader_textured = {
		"shaders/MVP_NORMALS_UVS.v.glsl",
		"shaders/DIFFUSE_TEXTURE.f.glsl",
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo_vertices);
	glGenBuffers(1, &vbo_normals);
	glGenBuffers(1, &vbo_uvs);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh.vertices.size(), &mesh.vertices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * mesh.normals.size(), &mesh.normals[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	if (!mesh.uvs.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * mesh.uvs.size(), &mesh.uvs[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	}
}

void Mesh_Renderer::draw_3D_textured(Mesh& mesh, const mat4& view_matrix, const mat4& projection_matrix, const Transform& transform, Texture& texture) {
	glBindVertexArray(vao);
	shader_textured.use();

	texture.use();

	shader_textured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));
	shader_textured.set_uniform("view", view_matrix);
	shader_textured.set_uniform("projection", projection_matrix);
	shader_textured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());

	shader_textured.release();
	glBindVertexArray(0);
}

void Mesh_Renderer::draw_3D_coloured(Mesh& mesh, const mat4& view_matrix, const mat4& projection_matrix, const Transform& transform, const vec4& colour) {
	glBindVertexArray(vao);
	shader_coloured.use();

	shader_coloured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));
	shader_coloured.set_uniform("view", view_matrix);
	shader_coloured.set_uniform("projection", projection_matrix);
	shader_coloured.set_uniform("uniform_colour", colour);
	shader_coloured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());

	shader_coloured.release();
	glBindVertexArray(0);
}

void Mesh_Renderer::destroy() {
	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &vbo_normals);
	glDeleteBuffers(1, &vbo_uvs);
	glDeleteVertexArrays(1, &vao);
	shader_coloured.destroy();
	shader_textured.destroy();
}