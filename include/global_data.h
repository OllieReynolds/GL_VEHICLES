#pragma once

#include "maths.h"

namespace utils {
	using namespace maths;

	namespace colour {
		extern vec4 black;
		extern vec4 white;
	};

	extern vec2 quad_points[4];
	extern vec4 quad_points_and_uvs[4];
	extern float cube_points[108];
	extern float cube_vertices_normals[216];
};