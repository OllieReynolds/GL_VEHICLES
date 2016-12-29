#pragma once

#include "maths.h"
#include "shader.h"
#include "texture.h"
#include "utils.h"

#include <glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace maths;
using namespace utils;

class Mesh_Renderer {
public:
	Mesh_Renderer(const char* filename = "");

	void init();
	void draw_3D_coloured(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour);
	void draw_3D_textured(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, Texture& texture);
	void destroy();

private:
	bool uvs_included;

	GLuint vao;
	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;

	utils::Shader shader_coloured;
	utils::Shader shader_textured;

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;

	void load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals, std::vector<vec2>& uvs);
	void process_face(std::string& line, bool uvs_included, std::vector<int>& vertex_indices, std::vector<int>& uv_indices, std::vector<int>& normal_indices);
	bool check_uvs_included(const char* filename);
};