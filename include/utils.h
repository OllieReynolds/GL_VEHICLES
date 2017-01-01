#pragma once

#include <chrono>
#include <random>
#include <fstream>
#include <intrin.h>

#include "maths.h"

namespace utils {
	using namespace maths;

	struct Transform {
		vec3 position;
		vec3 size;
		vec3 rotation;
	};

	struct dimensions {
		int width, height;
	};

	static dimensions png_dimensions(const char* filename) {
		std::string line;
		std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);

		file.seekg(16, std::ios_base::cur);

		__int32 width;
		__int32 height;

		file.read((char*)&width, 4);
		file.read((char*)&height, 4);

		file.close();

		return{ (int)_byteswap_ulong(width), (int)_byteswap_ulong(height) };
	}

	static float elapsed_time() {
		using namespace std::chrono;
		static time_point<steady_clock> start = steady_clock::now();
		return duration_cast<duration<float>>(steady_clock::now() - start).count();
	}

	static float gen_random(float min = 0.f, float max = 10.f) {
		static std::random_device rd;
		static std::mt19937 mt(rd());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(mt);
	}

	static vec2 gen_random(vec2 min, vec2 max) {
		float x = gen_random(min.x, max.x);
		float y = gen_random(min.y, max.y);
		return vec2(x, y);
	}

	static std::string friendly_float(const float f) {
		std::string str = std::to_string(f);
		int offset = (f < 0.f) ? 4 : 3;
		str.erase(str.begin() + offset, str.end());
		if (f >= 0.f)
			str = " " + str;
		return str;
	}

	static mat4 gen_model_matrix(const vec2& size, const vec2& position) {
		mat4 s = scale(vec3{size, 0.f});
		mat4 t = transpose(translate(vec3{position, 0.f}));
		return mult(s, t);
	}

	static mat4 gen_model_matrix(const vec3& size, const vec3& position, float rotation) {
		mat4 s = scale(size);
		mat4 t = transpose(translate(position));
		mat4 r = rotate_y(rotation);
		return mult(mult(s, r), t);
	}

	static mat4 gen_model_matrix(const vec3& size, const vec3& position, const vec3& rotation) {
		mat4 s = scale(size);
		mat4 t = transpose(translate(position));
		mat4 r = rotate(rotation);
		
		return mult(mult(s, r), t);
	}

	namespace data {
		namespace colour {
			extern vec4 black;
			extern vec4 white;
			extern vec4 red;
			extern vec4 green;
			extern vec4 blue;
			extern vec4 yellow;
			extern vec4 grey;
			extern vec4 dark_grey;
		};

		namespace mesh {
			extern float quad_points_textured[20];
			extern vec3 quad_points[4];
			extern vec4 quad_points_and_uvs[4];

			extern float cube_points[108];
			extern float cube_vertices_normals[216];
		}
	};

	namespace shared {
		static bool line_intersect(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p4) {
			// Store the values for fast access and easy
			// equations-to-code conversion
			double x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
			double y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

			double d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
			// If d is zero, there is no intersection
			if (d == 0) return false;

			// Get the x and y
			double pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
			double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
			double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

			// Check if the x and y coordinates are within both lines
			if (x < min(x1, x2) || x > max(x1, x2) ||
				x < min(x3, x4) || x > max(x3, x4)) return false;
			if (y < min(y1, y2) || y > max(y1, y2) ||
				y < min(y3, y4) || y > max(y3, y4)) return false;

			return true;
		}

		static mat4 perspective_matrix(float fov, float aspect, float near, float far) {
			float M_PI = 3.141592f;
			float D2R = M_PI / 180.0;
			float yScale = 1.0 / tan(D2R * fov / 2);
			float xScale = yScale / aspect;
			float nearmfar = near - far;

			return mat4{
				{ xScale, 0.f, 0.f, 0.f },
				{ 0.f, yScale , 0.f, 0.f },
				{ 0.f, 0.f, (far + near) / nearmfar, -1.f },
				{ 0.f, 0.f, far * near / nearmfar, 0.f }
			};
		}

	

		static mat4 view_matrix(vec3 eye, vec3 target, vec3 up) {
			vec3 zaxis = normalise(eye - target);    // The "forward" vector.
			vec3 xaxis = normalise(cross_product(up, zaxis));// The "right" vector.
			vec3 yaxis = cross_product(zaxis, xaxis);     // The "up" vector.

			mat4 viewMatrix = {
				vec4(xaxis.x,            yaxis.x,            zaxis.x,       0),
				vec4(xaxis.y,            yaxis.y,            zaxis.y,       0),
				vec4(xaxis.z,            yaxis.z,            zaxis.z,       0),
				vec4(-dot_product(xaxis, eye), -dot_product(yaxis, eye), -dot_product(zaxis, eye),  1)
			};

			return viewMatrix;
		}
	};
};