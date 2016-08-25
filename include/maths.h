#pragma once

#include <algorithm>
#include <cmath>
#include <ostream>
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <type_traits>

																													   
namespace maths {
	class vec2 {
	public:
		vec2() : n{0.f, 0.f} {}
		vec2(const float v) : n{v, v} {}
		vec2(const float x, const float y) : n{x, y} {}

		vec2& operator  = (const vec2& v) { x  = v.x; y  = v.y; return *this; }
		vec2& operator += (const vec2& v) { x += v.x; y += v.y; return *this; }
		vec2& operator -= (const vec2& v) { x -= v.x; y -= v.y; return *this; }
		vec2& operator *= (const float v) { x *= v; y *= v; return *this; }
		vec2& operator /= (const float v) { x /= v; y /= v; return *this; }

		inline       float& operator [] (int i)       { return n[i]; }
		inline const float& operator [] (int i) const { return n[i]; }

		friend bool operator == (const vec2& a, const vec2& b) { return a.x == b.x && a.y == b.y; }
		friend bool operator != (const vec2& a, const vec2& b) { return !(a == b); }

		friend vec2 operator + (const vec2& a, const vec2& b) { return { a.x + b.x, a.y + b.y }; }
		friend vec2 operator - (const vec2& a, const vec2& b) { return { a.x - b.x, a.y - b.y }; }
		friend vec2 operator * (const vec2& a, const float v) { return { a.x * v, a.y * v }; }
		friend vec2 operator / (const vec2& a, const float v) { return { a.x / v, a.y / v }; }

		friend std::ostream& operator << (std::ostream& os, const vec2& v) { os << "(" << v.x << ", " << v.y << ")"; return os; }

		union {
			float n[2];
			struct {
				float x;
				float y;
			};
		};
	};

	class vec3 {
	public:
		vec3() : n{0.f, 0.f, 0.f} {}
		vec3(const float v) : n{v, v, v} {}
		vec3(const float x, const float y, const float z) : n{x, y, z} {}
		
		vec3& operator  = (const vec3& v) { x  = v.x; y  = v.y; z  = v.z; return *this; }
		vec3& operator += (const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		vec3& operator -= (const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		vec3& operator *= (const float v) { x *= v; y *= v; z *= v; return *this; }
		vec3& operator /= (const float v) { x /= v; y /= v; z /= v; return *this; }

		inline       float& operator [] (int i)       { return n[i]; }
		inline const float& operator [] (int i) const { return n[i]; }

		friend bool operator == (const vec3& a, const vec3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
		friend bool operator != (const vec3& a, const vec3& b) { return !(a == b); }

		friend vec3 operator + (const vec3& a, const vec3& b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
		friend vec3 operator - (const vec3& a, const vec3& b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
		friend vec3 operator * (const vec3& a, const float v) { return { a.x * v, a.y * v, a.z * v }; }
		friend vec3 operator / (const vec3& a, const float v) { return { a.x / v, a.y / v, a.z / v }; }

		friend std::ostream& operator << (std::ostream& os, const vec3& v) { os << "(" << v.x << ", " << v.y << ", " << v.z << ")"; return os; }

		union {
			float n[3];
			struct {
				float x;
				float y;
				float z;
			};
		};
	};
	
	class vec4 {
	public:
		vec4() : n{0.f, 0.f, 0.f, 0.f} {}
		vec4(const float v) : n{v, v, v, v} {}
		vec4(const float x, const float y, const float z, const float w) : n{x, y, z, w} {}

		vec4& operator  = (const vec4& v) { x  = v.x; y  = v.y; z  = v.z; w  = v.w; return *this; }
		vec4& operator += (const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		vec4& operator -= (const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		vec4& operator *= (const float v) { x *= v; y *= v; z *= v; w *= v; return *this; }
		vec4& operator /= (const float v) { x /= v; y /= v; z /= v; w /= v; return *this; }

		inline       float& operator [] (int i)       { return n[i]; }
		inline const float& operator [] (int i) const { return n[i]; }

		friend bool operator == (const vec4& a, const vec4& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
		friend bool operator != (const vec4& a, const vec4& b) { return !(a == b); }

		friend vec4 operator + (const vec4& a, const vec4& b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
		friend vec4 operator - (const vec4& a, const vec4& b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
		friend vec4 operator * (const vec4& a, const float v) { return { a.x * v, a.y * v, a.z * v, a.w * v }; }
		friend vec4 operator / (const vec4& a, const float v) { return { a.x / v, a.y / v, a.z / v, a.w / v }; }

		friend std::ostream& operator << (std::ostream& os, const vec4& v) { os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")"; return os; }

		union {
			float n[4];
			struct {
				float x;
				float y;
				float z;
				float w;
			};
		};
	};

	class mat4 {
	public:
		mat4() : n{{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}} {}
		mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d) : n{a, b, c, d} {}

		mat4& operator = (const mat4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }

		inline       vec4& operator [] (int i)       { return n[i]; }
		inline const vec4& operator [] (int i) const { return n[i]; }

		friend std::ostream& operator << (std::ostream& os, const mat4& v) { os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")"; return os; }

		void scale(const vec3& v) { x.x = v.x; y.y = v.y; z.z = v.z; }
		void translate(const vec3& v) { z.x = v.x; z.y = v.y; z.z = v.z; }

		union {
			vec4 n[4];
			struct {
				vec4 x;
				vec4 y;
				vec4 z;
				vec4 w;
			};
		};
	};

	// Types for 2D and 3D Matrix

	struct line {
		vec2 A;
		vec2 B;
	};

	struct circle {
		vec2 O;
		float r;
	};

	struct segment {
		vec2 A;
		vec2 O;
		vec2 B;
	};

	bool almost_equal(float x, float y, float error_factor);

	bool check_clockwise(const line& l, const vec2& p);
	bool check_anticlockwise(const line& l, const vec2& p);

	vec3 cross_product(const vec3& a, const vec3& b);

	// Make this work for 2d Matrix
	float determinant(const vec2& a, const vec2& b);

	float distance(const vec2& a, const vec2& b);
	float distance(const vec3& a, const vec3& b);
	float distance(const vec4& a, const vec4& b);

	float dot_product(const vec2& a, const vec2& b);
	float dot_product(const vec3& a, const vec3& b);
	float dot_product(const vec4& a, const vec4& b);

	float magnitude(const vec2& v);
	float magnitude(const vec3& v);
	float magnitude(const vec4& v);

	vec2 normalise(const vec2& v);
	vec3 normalise(const vec3& v);
	vec4 normalise(const vec4& v);
	
	mat4 orthographic_matrix(const vec2& resolution, float nZ, float fZ, mat4 m);

	namespace intersections {
		bool point_circle(const vec2& P, const circle& C);
		bool point_segment(const vec2& P, const segment& S);
	}
}