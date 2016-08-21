#include "..\include\maths.h"

namespace maths {
	vec3 cross_product(const vec3& a, const vec3& b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	float dot_product(const vec2& a, const vec2& b) { return (a.x * b.x) + (a.y * b.y); }
	float dot_product(const vec3& a, const vec3& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }
	float dot_product(const vec4& a, const vec4& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w); }

	float magnitude(const vec2& v) { return sqrt(v.x * v.x + v.y * v.y); }
	float magnitude(const vec3& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }
	float magnitude(const vec4& v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); }

	template <class T> 
	T normalise(const T& v) { return (v == T{0.f}) ? v : v / magnitude(v); }

	mat4 orthographic_matrix(const vec2& resolution, float nZ, float fZ, mat4 m) {
		m.x.x = 2.f / resolution.x;
		m.y.y = 2.f / resolution.y;
		m.z.z = -2.f / (fZ - nZ);

		m.w.x = -resolution.x / resolution.x;
		m.w.y = -resolution.y / resolution.y;
		m.w.z = -(fZ + nZ) / (fZ - nZ);
		return m;
	}
}

