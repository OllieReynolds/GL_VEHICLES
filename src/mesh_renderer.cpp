#include "..\include\mesh_renderer.h"

void Mesh_Renderer::init(Model& model) {
	shader_coloured = {
		"shaders/MVP_NORMALS.v.glsl",
		"shaders/DIFFUSE.f.glsl",
	};
	
	shader_textured = {
		"shaders/MVP_NORMALS_UVS.v.glsl",
		"shaders/DIFFUSE_TEXTURE.f.glsl",
	};

	num_meshes = model.meshes.size();
	gl_data = new gl[num_meshes];

	for (int i = 0; i < model.meshes.size(); i++) {
		glGenVertexArrays(1, &gl_data[i].vao);
		glBindVertexArray(gl_data[i].vao);

		glGenBuffers(1, &gl_data[i].vbo_vertices);
		glGenBuffers(1, &gl_data[i].vbo_normals);
		glGenBuffers(1, &gl_data[i].vbo_uvs);

		glBindBuffer(GL_ARRAY_BUFFER, gl_data[i].vbo_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * model.meshes[i].vertices.size(), &model.meshes[i].vertices[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		glBindBuffer(GL_ARRAY_BUFFER, gl_data[i].vbo_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * model.meshes[i].normals.size(), &model.meshes[i].normals[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		if (!model.meshes[i].uvs.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, gl_data[i].vbo_uvs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * model.meshes[i].uvs.size(), &model.meshes[i].uvs[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		}
	}
	
}

void Mesh_Renderer::draw_3D_textured(Model& model, const Camera& camera, const Transform& transform, Texture& texture) {
	for (int i = 0; i < num_meshes; i++) {
		glBindVertexArray(gl_data[i].vao);
		shader_textured.use();

		texture.use();

		shader_textured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));
		shader_textured.set_uniform("view", camera.matrix_view);
		shader_textured.set_uniform("projection", camera.matrix_projection_persp);
		shader_textured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
		glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());

		shader_textured.release();
		glBindVertexArray(0);
	}
}

void Mesh_Renderer::draw_3D_coloured(Model& model, const Camera& camera, const Transform& transform, const vec4& colour) {
	
	for (int i = 0; i < num_meshes; i++) {
		glBindVertexArray(gl_data[i].vao);
		shader_coloured.use();

		shader_coloured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));
		shader_coloured.set_uniform("view", camera.matrix_view);
		shader_coloured.set_uniform("projection", camera.matrix_projection_persp);
		shader_coloured.set_uniform("uniform_colour", colour);
		shader_coloured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
		glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());

		shader_coloured.release();
		glBindVertexArray(0);
	}
}

void Mesh_Renderer::destroy() {
	for (int i = 0; i < num_meshes; i++) {
		glDeleteBuffers(1, &gl_data[i].vbo_vertices);
		glDeleteBuffers(1, &gl_data[i].vbo_normals);
		glDeleteBuffers(1, &gl_data[i].vbo_uvs);
		glDeleteVertexArrays(1, &gl_data[i].vao);
	}

	delete[] gl_data;

	shader_coloured.destroy();
	shader_textured.destroy();
}