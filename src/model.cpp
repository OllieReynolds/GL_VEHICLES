#include "..\include\model.h"

void Model::init(const char* filename) {
	int lc = line_count(filename);
	std::vector<int> preproc_data = file_preprocess(filename);
	std::vector <std::pair<int, int>> data_ranges = file_preprocess_full(preproc_data, lc);

	load_meshes(filename, data_ranges);

	//for (int i = 0; i < begin_end_markers.size(); i++) {
	//	Mesh mesh;
	//	load_model(filename, begin_end_markers[i], mesh);
	//	meshes.push_back(mesh);
	//}
}

void Model::load_meshes(const char* filename, const std::vector<std::pair<int, int>>& data_ranges) {
	std::string line;
	std::vector<vec3> vertex_list, normal_list, uv_list;

	std::ifstream ifs(filename, std::istream::in);

	bool uvs_included = check_uvs_included(filename);

	int line_num = 0;
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

		line_num++;
	}

	ifs.close();

	for (int i = 0; i < meshes.size(); i++) {
		for (int j = 0; j < meshes[i].vertex_indices.size(); j++) {
			int vert_index = meshes[i].vertex_indices[j];
			int norm_index = meshes[i].normal_indices[j];

			vec3 vertex = vertex_list[vert_index];
			vec3 normal = normal_list[norm_index];

			meshes[i].vertices.push_back(vertex);
			meshes[i].normals.push_back(normal);

			if (meshes[i].uvs_included) {
				int uv_index = meshes[i].uv_indices[j];
				vec2 uv = { uv_list[uv_index].x, 1.f - uv_list[uv_index].y };
				meshes[i].uvs.push_back(uv);
			}
		}
	}
}




void Model::load_model(const char* filename, const std::pair<int, int>& begin_end, Mesh& mesh) {
	std::string line;

	std::vector<vec3> temp_vertices;
	std::vector<vec3> temp_normals;
	std::vector<vec2> temp_uvs;

	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;
	std::vector<int> uv_indices;

	mesh.uvs_included = check_uvs_included(filename);

	std::ifstream ifs(filename, std::istream::in);

	int line_num = 0;
	while (std::getline(ifs, line)) {
		if(line_num >= begin_end.first && line_num <= begin_end.second) {
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
				process_face(line, mesh.uvs_included, vertex_indices, uv_indices, normal_indices);
			}
		}
		line_num++;
	}

	ifs.close();

	// TODO:
	// Change this to reflect master list of vertices, normals and uvs
	// Let faces draw from big list rather than sub list

	for (int i = 0; i < vertex_indices.size(); i++) {
		int vert_index = vertex_indices[i];
		int norm_index = normal_indices[i];

		vec3 vertex = temp_vertices[vert_index];
		vec3 normal = temp_normals[norm_index];
		
		mesh.vertices.push_back(vertex);
		mesh.normals.push_back(normal);

		if (mesh.uvs_included) {
			int uv_index = uv_indices[i];
			vec2 uv = { temp_uvs[uv_index].x, 1.f - temp_uvs[uv_index].y };
			mesh.uvs.push_back(uv);
		}
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


std::vector<std::pair<int, int>> Model::file_preprocess_full(std::vector<int>& preproc_data, int linecount) {
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

int Model::line_count(const char* filename) {
	int count = 0;
	std::string line;
	std::ifstream ifs(filename, std::istream::in);

	while (std::getline(ifs, line))
		count++;

	ifs.close();
	return count;
}

std::vector<int> Model::file_preprocess(const char* filename) {

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