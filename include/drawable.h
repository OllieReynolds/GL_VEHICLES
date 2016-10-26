#pragma once

#include <glew.h>

#include "maths.h"
#include "shader.h"

namespace simulation {
	using namespace maths;

	struct Transform {
		Transform(const vec2& position = 0.f, const vec2& size = 0.f, float rotation = 0.f) 
			: position(position), size(size), rotation(rotation) { }

		vec2 position;
		vec2 size;
		float rotation;
	};

	class Drawable {
	public:
		Drawable(const Transform& transform, const vec4& colour)
			: transform(transform.position, transform.size, transform.rotation), colour(colour) { }

		~Drawable() { }

		virtual void init() = 0;
		virtual void update(const maths::vec2& cursor_pos) = 0;
		virtual void draw() = 0;
		virtual void destroy() = 0;

		// OpenGL
		GLuint gl_array_object;
		GLuint gl_buffer_object;
		utils::Shader shader;

		// Transforms
		Transform transform;
		vec4 colour;
	};
}