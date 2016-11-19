#pragma once

#include "maths.h"

using namespace maths;

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
		
		return mat4 {
			{xScale, 0.f, 0.f, 0.f},
			{0.f, yScale , 0.f, 0.f},
			{0.f, 0.f, (far + near) / nearmfar, -1.f},
			{0.f, 0.f, far * near / nearmfar, 0.f}
		};
	}

	static mat4 perspective(float FOV, float width, float height, float near, float far) {
		float e = 1.f / (tan(FOV / 2.f));
		float a = height / width;

		return mat4{
			{e, 0.f, 0.f, 0.f},
			{0.f, e / a, 0.f, 0.f},
			{0.f, 0.f, -((far + near) / (far - near)), -((2 * far * near) / (far - near))},
			{0.f, 0.f, -1.f, 0.f}
		};
	}

	static mat4 view_matrix(vec3 eye, vec3 target, vec3 up) {
		vec3 zaxis = normalise(eye - target);    // The "forward" vector.
		vec3 xaxis = normalise(cross_product(up, zaxis));// The "right" vector.
		vec3 yaxis = cross_product(zaxis, xaxis);     // The "up" vector.

		// Create a 4x4 view matrix from the right, up, forward and eye position vectors
		mat4 viewMatrix = {
			vec4(xaxis.x,            yaxis.x,            zaxis.x,       0),
			vec4(xaxis.y,            yaxis.y,            zaxis.y,       0),
			vec4(xaxis.z,            yaxis.z,            zaxis.z,       0),
			vec4(-dot_product(xaxis, eye), -dot_product(yaxis, eye), -dot_product(zaxis, eye),  1)
		};

		return viewMatrix;
	}
};