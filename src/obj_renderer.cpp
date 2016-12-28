#include "..\include\obj_renderer.h"


Obj_Renderer::Obj_Renderer(const char* filename) {
	load_mesh(filename, vertices, normals);
}

void Obj_Renderer::init() {
	shader = {
		"shaders/MVP_NORMALS.v.glsl",
		"shaders/DIFFUSE.f.glsl",
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo_normals);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normals.size(), &normals[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
}

void Obj_Renderer::draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, float rotation, const vec4& colour) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	shader.use();

	shader.set_uniform("model", utils::gen_model_matrix(size, position, rotation));
	shader.set_uniform("view", view_matrix);
	shader.set_uniform("projection", projection_matrix);
	shader.set_uniform("uniform_colour", colour);
	shader.set_uniform("light_position", vec3{ 0.f, 30.f, 0.f });
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	shader.release();
	glBindVertexArray(0);
}

void Obj_Renderer::destroy() {
	shader.destroy();
}

void Obj_Renderer::load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals) {
	std::ifstream ifs(filename, std::istream::in);
	std::string line;
	std::vector<vec3> temp_vertices;
	std::vector<vec3> temp_normals;
	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;

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
		else if (type == "f") {
			int x_vert, y_vert, z_vert;
			int x_norm, y_norm, z_norm;

			const char* s = line.c_str();
			sscanf_s(s, "f %d//%d %d//%d %d//%d", &x_vert, &x_norm, &y_vert, &y_norm, &z_vert, &z_norm);

			vertex_indices.push_back(--x_vert);
			vertex_indices.push_back(--y_vert);
			vertex_indices.push_back(--z_vert);

			normal_indices.push_back(--x_norm);
			normal_indices.push_back(--y_norm);
			normal_indices.push_back(--z_norm);
		}
	}

	ifs.close();

	for (int i = 0; i < vertex_indices.size(); i++) {
		vec3 vertex = { temp_vertices[vertex_indices[i]].x, temp_vertices[vertex_indices[i]].y, temp_vertices[vertex_indices[i]].z };
		vec3 normal = { temp_normals[normal_indices[i]].x, temp_normals[normal_indices[i]].y, temp_normals[normal_indices[i]].z };
		vertices.push_back(vertex);
		normals.push_back(normal);
	}
}