#include "..\include\model.h"

Model::Model() {

}

void Model::init(const char* filename) {
	std::vector <std::pair<int, int>> data_ranges = file_preprocess(filename);
	load_meshes(filename, data_ranges);

	for (uint32_t i = 0; i < meshes.size(); i++) {
		glGenVertexArrays(1, &meshes[i].vao);
		glBindVertexArray(meshes[i].vao);

		glGenBuffers(1, &meshes[i].vbo_vertices);
		glGenBuffers(1, &meshes[i].vbo_normals);
		glGenBuffers(1, &meshes[i].vbo_uvs);

		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vbo_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * meshes[i].vertices.size(), &meshes[i].vertices[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vbo_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * meshes[i].normals.size(), &meshes[i].normals[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		if (!meshes[i].uvs.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, meshes[i].vbo_uvs);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * meshes[i].uvs.size(), &meshes[i].uvs[0], GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
		}
	}

}

void Model::destroy() {
	for (uint32_t i = 0; i < meshes.size(); i++) {
		glDeleteBuffers(1, &meshes[i].vbo_vertices);
		glDeleteBuffers(1, &meshes[i].vbo_normals);
		glDeleteBuffers(1, &meshes[i].vbo_uvs);
		glDeleteVertexArrays(1, &meshes[i].vao);
	}
}

void Model::load_meshes(const char* filename, const std::vector<std::pair<int, int>>& data_ranges) {
	std::string line;
	std::vector<vec3> vertex_list, normal_list;
	std::vector<vec2> uv_list;
	std::ifstream ifs(filename, std::istream::in);
	bool uvs_included = check_uvs_included(filename);

	int mesh_num = -1;
	while (std::getline(ifs, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "v") {
			float x, y, z;
			iss >> x >> y >> z;
			vertex_list.push_back({ x, y, z });
		}
		else if (type == "vn") {
			float x, y, z;
			iss >> x >> y >> z;
			normal_list.push_back({ x, y, z });
		}
		else if (type == "vt") {
			float u, v;
			iss >> u >> v;
			uv_list.push_back({ u, v });
		}
		else if (type == "f") {
			process_face(line, uvs_included, meshes.back().vertex_indices, meshes.back().uv_indices, meshes.back().normal_indices);
		}
		else if (type == "o") {
			Mesh mesh = Mesh();
			mesh.uvs_included = uvs_included;
			meshes.push_back(mesh);
			mesh_num++;
		}
	}

	ifs.close();

#ifdef _DEBUG
	std::cout << " Mesh Count: " << meshes.size() << std::endl;
	std::cout << "Vertex List: " << vertex_list.size() << std::endl;
	std::cout << "Normal List: " << normal_list.size() << std::endl;
	std::cout << "    UV List: " << uv_list.size() << std::endl;
#endif

	for (uint32_t i = 0; i < meshes.size(); i++) {
		for (uint32_t j = 0; j < meshes[i].vertex_indices.size(); j++) {
			int vert_index = meshes[i].vertex_indices[j];
			int norm_index = meshes[i].normal_indices[j];

			vec3 vertex = vertex_list[vert_index];
			vec3 normal = normal_list[norm_index];

			meshes[i].vertices.push_back(vertex);
			meshes[i].normals.push_back(normal);
		}

		if (meshes[i].uvs_included) {
			for (uint32_t j = 0; j < meshes[i].uv_indices.size(); j++) {
				int uv_index = meshes[i].uv_indices[j];
				vec2 uv = { uv_list[uv_index].x, 1.f - uv_list[uv_index].y };
				meshes[i].uvs.push_back(uv);
			}
		}

#ifdef _DEBUG
		std::cout << "     Mesh " << i << ":" << std::endl;
		std::cout << "Vertices: " << meshes[i].vertices.size() << std::endl;
		std::cout << " Normals: " << meshes[i].normals.size() << std::endl;
		std::cout << "     UVs: " << meshes[i].uvs.size() << std::endl;
#endif
	}
}

void Model::process_face(std::string& line, bool uvs_included, std::vector<int>& vertex_indices, std::vector<int>& uv_indices, std::vector<int>& normal_indices) {
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

bool Model::check_uvs_included(const char* filename) {
	std::string line;
	std::ifstream ifs(filename, std::istream::in);

	size_t pos;
	bool b = false;
	while (std::getline(ifs, line)) {
		pos = line.find('f');
		if (pos != std::string::npos && pos == 0) {
			char last = '1';
			for (uint32_t i = 0; i < line.length(); i++) {
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


std::vector<std::pair<int, int>> Model::file_preprocess(const char* filename) {
	std::vector<int> data_ranges;

	int current_line_number = 0;
	std::string line;
	std::ifstream ifs(filename, std::istream::in);
	while (std::getline(ifs, line)) {
		current_line_number++;

		if (line.front() == 'o')
			data_ranges.push_back(current_line_number);
	}
	ifs.close();

	int linecount = utils::line_count(filename);
	std::vector <std::pair<int, int>> begin_end_markers;
	for (uint32_t i = 0; i < data_ranges.size(); i++) {
		std::pair<int, int> marker;

		if (i == 0)
			marker.first = 3;
		else
			marker.first = data_ranges[i];

		if (i == data_ranges.size() - 1)
			marker.second = linecount - 1;
		else
			marker.second = data_ranges[i + 1] - 2;

		begin_end_markers.push_back(marker);
	}

	return begin_end_markers;
}