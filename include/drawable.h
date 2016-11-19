#pragma once

#include <glew.h>

#include "maths.h"
#include "shader.h"

namespace simulation {
	using namespace maths;

	struct Transform {
		vec3 position;
		vec3 size;
		float rotation;
	};

	class Drawable {
	public:
		Drawable(const Transform& t, const vec4& colour)
			: position(t.position), size(t.size), rotation(t.rotation), colour(colour) { }

		~Drawable() { }

		virtual void init() = 0;
		virtual void update(const maths::vec2& cursor_pos) = 0;
		virtual void draw() = 0;
		virtual void destroy() = 0;

		void set_gl_buffer_data(GLsizeiptr size, const void* data) {
			glGenVertexArrays(1, &gl_array_object);
			glBindVertexArray(gl_array_object);

			glGenBuffers(1, &gl_buffer_object);
			glBindBuffer(GL_ARRAY_BUFFER, gl_buffer_object);

			glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
		}

		mat4 gen_model_matrix() {
			mat4 s = scale(size);
			mat4 t = transpose(translate(position));
			mat4 r = rotate_y(rotation);
			return mult(mult(s, r), t);
		}

		mat4 gen_model_matrix(const vec2& size, const vec2& position) {
			mat4 s = scale(vec3{size, 0.f});
			mat4 t = transpose(translate(vec3{position, 0.f}));
			return mult(s, t);
		}

		GLuint gl_array_object;
		GLuint gl_buffer_object;
		utils::Shader shader;

		vec3 position;
		vec3 size;
		float rotation;
		vec4 colour;
	};
}