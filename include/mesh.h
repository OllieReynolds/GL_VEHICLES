#pragma once

#include "maths.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

using namespace maths;
using namespace utils;


struct Mesh {
	Mesh() { }
	void init(const char* filename);

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;

	bool uvs_included;

private:
	void load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals, std::vector<vec2>& uvs);

	int line_count(const char* filename);
	void process_face(std::string& line, bool uvs_included, std::vector<int>& vertex_indices, std::vector<int>& uv_indices, std::vector<int>& normal_indices);
	bool check_uvs_included(const char* filename);
	std::vector<int> file_preprocess(const char* filename);
	std::vector<std::pair<int, int>> file_preprocess_full(std::vector<int>& preproc_data, int linecount);

};