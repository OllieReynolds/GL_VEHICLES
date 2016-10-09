#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glew.h>

#include "maths.h"
#include "shader.h"

namespace simulation {
	using namespace maths;
	using namespace utils;

	struct Glyph {
		GLuint data;
		FT_Pos next_glyph_offset;
		vec2 glyph_size;
		vec2 bearing_offset;
	};

	class Text {
	public:
		Text(int pixel_size = 24) : pixel_size(pixel_size) {
		}

		void init_text();
		void draw_text(const std::string& msg, const vec2& position);
		void destroy_text();


	private:
		GLuint vao;
		GLuint vbo;
		Shader shader;

		int pixel_size;
		std::map<GLchar, Glyph> glyph_map;
	};
}