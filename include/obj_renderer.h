#pragma once

#include "maths.h"
#include "shader.h"
#include "utils.h"

#include <glew.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace maths;
using namespace utils;

class Obj_Renderer {
public:
	Obj_Renderer(const char* filename = "");

	void init();
	void draw_3D(const mat4& view_matrix, const mat4& projection_matrix, const vec3& position, const vec3& size, float rotation, const vec4& colour);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	GLuint vbo_normals;

	utils::Shader shader;

	std::vector<vec3> vertices;
	std::vector<vec3> normals;

	void load_mesh(const char* filename, std::vector<vec3>& vertices, std::vector<vec3>& normals);
};