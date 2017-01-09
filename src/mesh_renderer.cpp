#include "..\include\mesh_renderer.h"

void Mesh_Renderer::init() {
	shader_coloured = {
		"shaders/MVP_NORMALS.v.glsl",
		"shaders/DIFFUSE.f.glsl",
	};
	
	shader_textured = {
		"shaders/MVP_NORMALS_UVS.v.glsl",
		"shaders/DIFFUSE_TEXTURE.f.glsl",
	};
}

void Mesh_Renderer::draw_multiple_3D_textured(int n, Model& model, const Camera& camera, const Transform* transform, Texture& texture) {
	shader_textured.use();
	shader_textured.set_uniform("view", camera.matrix_view);
	shader_textured.set_uniform("projection", camera.matrix_projection_persp);
	shader_textured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });

	texture.use();

	for (int j = 0; j < n; j++) {
		shader_textured.set_uniform("model", gen_model_matrix(transform[j].size, transform[j].position, transform[j].rotation));

		for (int i = 0; i < model.meshes.size(); i++) {
			glBindVertexArray(model.meshes[i].vao);
			glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());
			glBindVertexArray(0);
		}
	}

	shader_textured.release();
}

void Mesh_Renderer::draw_3D_textured(Model& model, const Camera& camera, const Transform& transform, Texture& texture) {
	shader_textured.use();
	shader_textured.set_uniform("view", camera.matrix_view);
	shader_textured.set_uniform("projection", camera.matrix_projection_persp);
	shader_textured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	shader_textured.set_uniform("model", gen_model_matrix(transform.size, transform.position, transform.rotation));

	texture.use();
	
	for (int i = 0; i < model.meshes.size(); i++) {
		glBindVertexArray(model.meshes[i].vao);
		glDrawArrays(GL_TRIANGLES, 0, model.meshes[i].vertices.size());
		glBindVertexArray(0);
	}

	shader_textured.release();
}

void Mesh_Renderer::draw_3D_coloured(Model& model, const Camera& camera, const Transform& transform, const vec4& colour) {
	
	for (int i = 0; i < model.meshes.size(); i++) {
		glBindVertexArray(model.meshes[i].vao);
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
	shader_coloured.destroy();
	shader_textured.destroy();
}