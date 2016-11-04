#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glew.h>

#include "maths.h"
#include "shader.h"

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
	Text(int pixel_size = 24, const std::string& font = "", const vec4& colour = 0.f) 
		: pixel_size(pixel_size), font(font), colour(colour) 
	{
	}

	void init_text(const vec2& screen_resolution);
	void draw_text(const std::string& msg, const vec2& position, bool centered);
	void destroy_text();

	int pixel_size;
	std::string font;
	vec4 colour;

private:
	GLuint vao;
	GLuint vbo;
	Shader shader;

	std::map<GLchar, Glyph> glyph_map;
};