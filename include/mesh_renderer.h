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

	void init();
	void draw_3D_coloured(Model& mesh, const Camera& camera, const Transform& transform, const vec4& colour);
	void draw_3D_textured(Model& mesh, const Camera& camera, const Transform& transform, Texture& texture);
	void destroy();

private:
	Shader shader_coloured;
	Shader shader_textured;
};