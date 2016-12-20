#pragma once

#include "maths.h"

namespace utils {
	using namespace maths;

	namespace colour {
		extern vec4 black;
		extern vec4 white;
		extern vec4 red;
		extern vec4 green;
		extern vec4 blue;
		extern vec4 grey;
		extern vec4 dark_grey;
	};

	extern float quad_points_textured[20];
	extern vec3 quad_points[4];
	extern vec4 quad_points_and_uvs[4];


	extern float cube_points[108];
	extern float cube_vertices_normals[216];
};