#pragma once

#include "maths.h"

namespace shared {

	using maths::vec2;
	using maths::min;
	using maths::max;

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
};