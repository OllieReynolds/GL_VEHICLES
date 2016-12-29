#include "..\include\mesh_renderer.h"

Mesh_Renderer::Mesh_Renderer(const char* filename) {
	load_mesh(filename, vertices, normals, uvs);
}

void Mesh_Renderer::init() {
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

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), &normals[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

	if (!uvs.empty()) {
		glGenBuffers(1, &vbo_uvs);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * uvs.size(), &uvs[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	}
}

void Mesh_Renderer::draw_3D_textured(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, Texture& texture) {
	glBindVertexArray(vao);
	shader_textured.use();

	texture.use();

	shader_textured.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
	shader_textured.set_uniform("view", view_matrix);
	shader_textured.set_uniform("projection", projection_matrix);
	shader_textured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	shader_textured.release();
	glBindVertexArray(0);
}

void Mesh_Renderer::draw_3D_coloured(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour) {
	glBindVertexArray(vao);
	shader_coloured.use();

	shader_coloured.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
	shader_coloured.set_uniform("view", view_matrix);
	shader_coloured.set_uniform("projection", projection_matrix);
	shader_coloured.set_uniform("uniform_colour", colour);
	shader_coloured.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	shader_coloured.release();
	glBindVertexArray(0);
}

void Mesh_Renderer::destroy() {
	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &vbo_normals);
	if (uvs_included)
		glDeleteBuffers(1, &vbo_uvs);
	glDeleteVertexArrays(1, &vao);
	shader_coloured.destroy();
}

void Mesh_Renderer::load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals, std::vector<vec2>& uvs) {
	std::string line;
	
	std::vector<vec3> temp_vertices;
	std::vector<vec3> temp_normals;
	std::vector<vec2> temp_uvs;

	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;
	std::vector<int> uv_indices;

	uvs_included = check_uvs_included(filename);

	std::ifstream ifs(filename, std::istream::in);

	while (std::getline(ifs, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "v") {
			float x, y, z;
			iss >> x >> y >> z;
			temp_vertices.push_back({ x, y, z });
		}
		else if (type == "vn") {
			float x, y, z;
			iss >> x >> y >> z;
			temp_normals.push_back({ x, y, z });
		}
		else if (type == "vt") {
			float u, v;
			iss >> u >> v;
			temp_uvs.push_back({ u, v });
		}
		else if (type == "f") {
			process_face(line, uvs_included, vertex_indices, uv_indices, normal_indices);
		}
	}

	ifs.close();

	for (int i = 0; i < vertex_indices.size(); i++) {
		vec3 vertex = { temp_vertices[vertex_indices[i]].x, temp_vertices[vertex_indices[i]].y, temp_vertices[vertex_indices[i]].z };
		vec3 normal = { temp_normals[normal_indices[i]].x, temp_normals[normal_indices[i]].y, temp_normals[normal_indices[i]].z };
		vertices.push_back(vertex);
		normals.push_back(normal);

		if (uvs_included) {
			vec2 uv = { temp_uvs[uv_indices[i]].x, 1.f - temp_uvs[uv_indices[i]].y };
			uvs.push_back(uv);
		}
	}
}

void Mesh_Renderer::process_face(std::string& line, bool uvs_included, std::vector<int>& vertex_indices, std::vector<int>& uv_indices, std::vector<int>& normal_indices) {
	int x_vert, y_vert, z_vert;
	int x_norm, y_norm, z_norm;

	const char* s = line.c_str();
	
	if (uvs_included) {
		int x_uv, y_uv, z_uv;

		sscanf_s(s, "f %d/%d/%d %d/%d/%d %d/%d/%d", &x_vert, &x_uv, &x_norm, &y_vert, &y_uv, &y_norm, &z_vert, &z_uv, &z_norm);

		uv_indices.push_back(--x_uv);
		uv_indices.push_back(--y_uv);
		uv_indices.push_back(--z_uv);
	}
	else {
		sscanf_s(s, "f %d//%d %d//%d %d//%d", &x_vert, &x_norm, &y_vert, &y_norm, &z_vert, &z_norm);
	}

	vertex_indices.push_back(--x_vert);
	vertex_indices.push_back(--y_vert);
	vertex_indices.push_back(--z_vert);

	normal_indices.push_back(--x_norm);
	normal_indices.push_back(--y_norm);
	normal_indices.push_back(--z_norm);
}

bool Mesh_Renderer::check_uvs_included(const char* filename) {
	std::string line;
	std::ifstream ifs(filename, std::istream::in);

	size_t pos;
	bool b = false;
	while (std::getline(ifs, line)) {

		pos = line.find('f');
		if (pos != std::string::npos && pos == 0) {
			char last = '1';
			for (int i = 0; i < line.length(); i++) {
				char c = line.at(i);
				if (c == '/' && last == '/')
					return false;
				else
					last = c;
			}

			return true;
		}
	}

	ifs.close();
	return b;
}