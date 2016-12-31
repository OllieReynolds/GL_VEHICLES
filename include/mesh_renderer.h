#pragma once

#include "maths.h"
#include "model.h"
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

	// This should be invoked from a mesh renderer and take mesh type objects, but update to Model types for now.
	// E.g. 
	//
	// Model_Renderer.hpp
	// for (Mesh m: meshes) {
	//     mesh_renderer.draw()
	//
	// Should work for 1 mesh or 8 meshes

	void init(Model& mesh);
	void draw_3D_coloured(Model& mesh, const mat4& view_matrix, const mat4& projection_matrix, const Transform& transform, const vec4& colour);
	void draw_3D_textured(Model& mesh, const mat4& view_matrix, const mat4& projection_matrix, const Transform& transform, Texture& texture);
	void destroy();

private:
	bool uvs_included;

	struct gl {
		GLuint vao;
		GLuint vbo_vertices;
		GLuint vbo_normals;
		GLuint vbo_uvs;
	};
	
	int num_meshes;
	gl* gl_data;

	Shader shader_coloured;
	Shader shader_textured;
};