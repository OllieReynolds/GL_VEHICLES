#pragma once

#include "maths.h"
#include "mesh.h"
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
	Mesh_Renderer() { }

	void init(Mesh& mesh);
	void draw_3D_coloured(Mesh& mesh, const mat4& view_matrix, const mat4& projection_matrix, const Transform& transform, const vec4& colour);
	void draw_3D_textured(Mesh& mesh, const mat4& view_matrix, const mat4& projection_matrix, const Transform& transform, Texture& texture);
	void destroy();

private:
	bool uvs_included;

	GLuint vao;
	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;

	Shader shader_coloured;
	Shader shader_textured;
};