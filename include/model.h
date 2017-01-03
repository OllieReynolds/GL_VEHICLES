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
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;
	bool uvs_included;

	std::vector<int> vertex_indices, normal_indices, uv_indices;

	GLuint vao;
	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;
};

struct Model {
	Model() { }
	void init(const char* filename);
	void destroy();

	std::vector<Mesh> meshes;

private:
	void load_meshes(const char* filename, const std::vector<std::pair<int, int>>& data_ranges);
	void process_face(std::string& line, bool uvs_included, std::vector<int>& vertex_indices, std::vector<int>& uv_indices, std::vector<int>& normal_indices);
	bool check_uvs_included(const char* filename);
	std::vector<std::pair<int, int>> file_preprocess(const char* filename);

};