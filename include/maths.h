#pragma once

namespace maths {

	class vec2 {
	protected:
		float n[2];

	public:
		vec2() : n{0.f, 0.f} {}
		vec2(const float v) : n{v, v} {}
		vec2(const float x, const float y) : n{x, y} {}

		vec2& operator = (const vec2& v)  {
			n[0] = v[0]; 
			n[1] = v[1]; 
			return *this;
		}

		vec2& operator += (const vec2& v) {
			n[0] += v[0]; 
			n[1] += v[1]; 
			return *this;
		}

		vec2& operator -= (const vec2& v) { 
			n[0] -= v[0]; 
			n[1] -= v[1]; 
			return *this; 
		}

		vec2& operator *= (const float v) { 
			n[0] *= v; 
			n[1] *= v; 
			return *this; 
		}

		vec2& operator /= (const float v) { 
			n[0] /= v; 
			n[1] /= v; 
			return *this; 
		}

		inline       float& operator [] (int i)       { return n[i]; }
		inline const float& operator [] (int i) const { return n[i]; }

		friend int operator != (const vec2& a, const vec2& b) { return !(a == b); }
		friend int operator == (const vec2& a, const vec2& b) { 
			return (
				a[0] == b[0] && 
				a[1] == b[1]
			);
		}

		friend vec2 operator + (const vec2& a, const vec2& b) {
			return {
				a[0] + b[0], 
				a[1] + b[1]
			};
		}

		friend vec2 operator - (const vec2& a, const vec2& b) {
			return {
				a[0] - b[0], 
				a[1] - b[1]
			};
		}

		friend vec2 operator * (const vec2& a, const float v) {
			return {
				a[0] * v, 
				a[1] * v
			};
		}

		friend vec2 operator / (const vec2& a, const float v) {
			return {
				a[0] / v, 
				a[1] / v
			};
		}

		friend float magnitude_squared(const vec2& v);
	};

	static float magnitude_squared(const vec2& v) {
		return 
			v.n[0] * v.n[0] + 
			v.n[1] * v.n[1];
	}

	static float magnitude(const vec2& v) {
		return sqrt(magnitude_squared(v));
	}

	static vec2 normalise(const vec2& v) {
		return (v == vec2{0.f, 0.f}) ? v : v / magnitude(v);
	}




	class vec3 {
	protected:
		float n[3];

	public:
		vec3() : n{0.f, 0.f, 0.f} {}
		vec3(const float v) : n{v, v, v} {}
		vec3(const float x, const float y, const float z) : n{x, y, z} {}
		
		vec3& operator = (const vec3& v) { 
			n[0] = v[0]; 
			n[1] = v[1]; 
			n[2] = v[2]; 
			return *this; 
		}

		vec3& operator += (const vec3& v) { 
			n[0] += v[0]; 
			n[1] += v[1]; 
			n[2] += v[2]; 
			return *this; 
		}

		vec3& operator -= (const vec3& v) { 
			n[0] -= v[0]; 
			n[1] -= v[1];
			n[2] -= v[2]; 
			return *this; 
		}

		vec3& operator *= (const float& v) {
			n[0] *= v;
			n[1] *= v;
			n[2] *= v;
			return *this;
		}

		vec3& operator /= (const float v) {
			n[0] /= v;
			n[1] /= v;
			n[2] /= v;
			return *this;
		}

		inline       float& operator [] (int i)       { return n[i]; }
		inline const float& operator [] (int i) const { return n[i]; }

		friend int operator != (const vec3& a, const vec3& b) { return !(a == b); }
		friend int operator == (const vec3& a, const vec3& b) {
			return (
				a[0] == b[0] && 
				a[1] == b[1] && 
				a[2] == b[2]);
		}

		friend vec3 operator + (const vec3& a, const vec3& b) {
			return {
				a[0] + b[0],
				a[1] + b[1],
				a[2] + b[2]
			};
		}

		friend vec3 operator - (const vec3& a, const vec3& b) {
			return {
				a[0] - b[0],
				a[1] - b[1],
				a[2] - b[2]
			};
		}

		friend vec3 operator * (const vec3& a, const float v) {
			return {
				a[0] * v, 
				a[1] * v, 
				a[2] * v
			};
		}

		friend vec3 operator / (const vec3& a, const float v) {
			return {
				a[0] / v,
				a[1] / v,
				a[2] / v
			};
		}

		friend float magnitude_squared(const vec3& v);
	};

	static float magnitude_squared(const vec3& v) {
		return 
			v.n[0] * v.n[0] + 
			v.n[1] * v.n[1] + 
			v.n[2] * v.n[2];
	}

	static float magnitude(const vec3& v) {
		return sqrt(magnitude_squared(v));
	}

	static vec3 normalise(const vec3& v) {
		return (v == vec3{0.f, 0.f, 0.f}) ? v : v / magnitude(v);
	}




	class vec4 {
	protected:
		float n[4];

	public:
		vec4() : n{0.f, 0.f, 0.f, 0.f} {}
		vec4(const float v) : n{v, v, v, v} {}
		vec4(const float x, const float y, const float z, const float w) : n{x, y, z, w} {}

		vec4& operator = (const vec4& v) { 
			n[0] = v[0];  
			n[1] = v[1];  
			n[2] = v[2];  
			n[3] = v[3]; 
			return *this; 
		}

		vec4& operator += (const vec4& v) { 
			n[0] += v[0]; 
			n[1] += v[1]; 
			n[2] += v[2]; 
			n[3] += v[3]; 
			return *this; 
		}

		vec4& operator -= (const vec4& v) { 
			n[0] -= v[0]; 
			n[1] -= v[1]; 
			n[2] -= v[2]; 
			n[3] -= v[3]; 
			return *this; 
		}

		vec4& operator *= (const float& v) {
			n[0] *= v;
			n[1] *= v;
			n[2] *= v;
			n[3] *= v;
			return *this;
		}

		vec4& operator /= (const float v) {
			n[0] /= v;
			n[1] /= v;
			n[2] /= v;
			n[3] /= v;
			return *this;
		}

		inline       float& operator [] (int i)       { return n[i]; }
		inline const float& operator [] (int i) const { return n[i]; }

		friend int operator != (const vec4& a, const vec4& b) { return !(a == b); }
		friend int operator == (const vec4& a, const vec4& b) {
			return (
				a[0] == b[0] &&
				a[1] == b[1] &&
				a[2] == b[2] &&
				a[3] == b[3]);
		}

		friend vec4 operator + (const vec4& a, const vec4& b) {
			return {
				a[0] + b[0],
				a[1] + b[1],
				a[2] + b[2],
				a[3] + b[3]
			};
		}

		friend vec4 operator - (const vec4& a, const vec4& b) {
			return {
				a[0] - b[0],
				a[1] - b[1],
				a[2] - b[2],
				a[3] - b[3]
			};
		}

		friend vec4 operator * (const vec4& a, const float v) {
			return {
				a[0] * v,
				a[1] * v,
				a[2] * v,
				a[3] * v
			};
		}

		friend vec4 operator / (const vec4& a, const float v) {
			return {
				a[0] / v,
				a[1] / v,
				a[2] / v,
				a[3] / v
			};
		}

		friend float magnitude_squared(const vec4& v);
	};

	static float magnitude_squared(const vec4& v) {
		return 
			v.n[0] * v.n[0] + 
			v.n[1] * v.n[1] + 
			v.n[2] * v.n[2] +
			v.n[3] * v.n[3];
	}

	static float magnitude(const vec4& v) {
		return sqrt(magnitude_squared(v));
	}

	static vec4 normalise(const vec4& v) {
		return (v == vec4{0.f, 0.f, 0.f, 0.f}) ? v : v / magnitude(v);
	}




	class mat4 {
	protected:
		vec4 n[4];

	public:
		mat4() : n{{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, 0},{0, 0, 0, 1}} {}
		mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d) : n{a, b, c, d} {}

		mat4& operator = (const mat4& v) {
			n[0] = v[0];
			n[1] = v[1];
			n[2] = v[2];
			n[3] = v[3];
			return *this;
		}

		inline       vec4& operator [] (int i)       { return n[i]; }
		inline const vec4& operator [] (int i) const { return n[i]; }
	};

	static mat4 translate_matrix(const vec3& v) {
		return mat4(
			vec4(1.0, 0.0, 0.0, 0.0),
			vec4(0.0, 1.0, 0.0, 0.0),
			vec4(0.0, 0.0, 1.0, 0.0),
			vec4(v[0], v[1], v[2], 1.0)
		);
	}

	static mat4 scale_matrix(const vec3& v) {
		return mat4(
			vec4(v[0], 0.0, 0.0, 0.0),
			vec4(0.0, v[1], 0.0, 0.0),
			vec4(0.0, 0.0, v[2], 0.0),
			vec4(0.0, 0.0, 0.0, 1.0)
		);
	}

	static const mat4 orthographic_perspective(float w, float h, float nZ, float fZ) {
		float z = fZ - nZ;
		float i = -w / w;
		float j = -h / h;
		float k = -(fZ + nZ) / z;

		return mat4(
			vec4(2.f / w, 0.f, 0.f, 0.f),
			vec4(0.f, 2.f / h, 0.f, 0.f),
			vec4(0.f, 0.f, -2.f / z, 0.f),
			vec4(i, j, k, 1.f)
		);
	}
}
