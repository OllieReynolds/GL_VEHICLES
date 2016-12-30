#include "..\include\mesh.h"



void Mesh::init(const char* filename) {
	/*int lc = line_count(filename);
	std::vector<int> preproc_data = file_preprocess(filename);
	std::vector <std::pair<int, int>> begin_end_markers = file_preprocess_full(preproc_data, lc);*/

	load_mesh(filename, vertices, normals, uvs);
}

std::vector<std::pair<int, int>> Mesh::file_preprocess_full(std::vector<int>& preproc_data, int linecount) {
	std::vector <std::pair<int, int>> begin_end_markers;
	for (int i = 0; i < preproc_data.size(); i++) {
		std::pair<int, int> marker;

		if (i == 0)
			marker.first = 3;
		else
			marker.first = preproc_data[i];

		if (i == preproc_data.size() - 1)
			marker.second = linecount - 1;
		else
			marker.second = preproc_data[i + 1] - 2;

		begin_end_markers.push_back(marker);
	}

	return begin_end_markers;
}

int Mesh::line_count(const char* filename) {
	int count = 0;
	std::string line;
	std::ifstream ifs(filename, std::istream::in);

	while (std::getline(ifs, line)) {
		count++;
	}

	ifs.close();
	return count;
}

std::vector<int> Mesh::file_preprocess(const char* filename) {

	std::vector<int> preproc_data;

	int current_line_number = 0;

	std::string line;
	std::ifstream ifs(filename, std::istream::in);
	
	while (std::getline(ifs, line)) {
		current_line_number++;

		if (line.front() == 'o')
			preproc_data.push_back(current_line_number);
	}

	ifs.close();
	return preproc_data;
}


void Mesh::load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals, std::vector<vec2>& uvs) {
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

void Mesh::process_face(std::string& line, bool uvs_included, std::vector<int>& vertex_indices, std::vector<int>& uv_indices, std::vector<int>& normal_indices) {
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

bool Mesh::check_uvs_included(const char* filename) {
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