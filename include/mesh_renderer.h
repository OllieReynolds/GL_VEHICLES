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
	Mesh_Renderer(const char* filename, const char* tex_filename);

	void init();
	void draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour);
	void destroy();

	void tex_init(const char* tex_filename);
	void tex_draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, const vec3& rotation, const vec4& colour);
	void tex_destroy();

private:
	GLuint vao;
	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;

	utils::Shader shader;

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec2> uvs;

	Texture texture;

	void load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals);
	void load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals, std::vector<vec2>& uvs);
};