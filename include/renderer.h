#pragma once

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glew.h>

#include "camera.h"
#include "maths.h"
#include "model.h"
#include "shader.h"
#include "utils.h"
#include "texture.h"

using namespace maths;
using namespace utils;

class Circle_Renderer {
public:
	Circle_Renderer() { }

	void init();
	void draw_2D(const Camera& camera, const vec2& position, const vec2& size, const vec4& colour, bool filled);
	void draw_3D(const Camera& camera, const Transform& transform, const vec4& colour, bool filled);
	void draw_3D_shadow(const Camera& camera, const Transform& transform);
	void draw_multiple_3D_shadow(const Camera& camera, const std::vector<Transform>& transform_list);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_2D;
	Shader shader_3D;
	Shader shader_3D_shadow;
};

class Triangle_Renderer {
public:
	Triangle_Renderer() {}

	void init();
	void draw_3D_coloured(const Camera& camera, const vec3& a, const vec3& b, const vec3& c, const vec4& colour);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_3D_coloured;
};

class Quad_Renderer {
public:
	Quad_Renderer() {}

	void init();
	void draw_2D(const Camera& camera, const vec2& position, const vec2& size, const vec4& colour);
	void draw_3D_coloured(const Camera& camera, const Transform& transform, const vec4& colour);
	void draw_multiple_3D_coloured(const Camera& camera, const std::vector<Transform>& transform_list, const vec4& colour);
	void draw_2D_textured(const Camera& camera, const vec2& position, const vec2& size, Texture& tex);
	void draw_3D_textured(const Camera& camera, const Transform& transform, Texture& tex);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	Shader shader_2D;
	Shader shader_3D_textured;
	Shader shader_3D_coloured;
};

class Cube_Renderer {
public:
	Cube_Renderer() { }

	void init();
	void draw(const Camera& camera, const vec3& position, const vec3& size, float rotation, const vec4& colour);
	void draw_multiple(const Camera& camera, std::map<int, Transform>& transform_list, std::map<int, Vehicle_Attributes>& vehicle_attributes, std::map<int, Light>& lights);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	utils::Shader shader;
};

class Line_Renderer {
public:
	Line_Renderer() { }

	void init();
	void draw(const Camera& camera, const vec3& world_space_a, const vec3& world_space_b, const vec4& colour);
	void destroy();

private:
	GLuint vao;
	GLuint vbo;
	utils::Shader shader;
};

class Model_Renderer {
public:
	Model_Renderer() { }

	void init();
	void draw_3D_coloured(Model& model, const Camera& camera, const Transform& transform, const vec4& colour);
	void draw_3D_textured(Model& model, const Camera& camera, const Transform& transform, Texture& texture);
	void draw_multiple_3D_textured(int n, Model& model, const Camera& camera, const std::vector<Transform>& transform_list, Texture& texture, std::map<int, Light>& lights);
	void draw_multiple_3D_textured(int n, Model& model, const Camera& camera, std::map<int, std::vector<Transform>>& transform_list, Texture& texture, std::map<int, Light>& lights);
	void destroy();

private:

	Shader shader_coloured;
	Shader shader_textured;
};

class Text_Renderer {
public:
	Text_Renderer(int pixel_size = 24, const std::string& font = "") : pixel_size(pixel_size), font(font) { }

	void init(const vec2& screen_resolution);
	void draw(const std::string& msg, const vec2& position, bool centered, const vec4& colour);
	void destroy();

	int pixel_size;
	std::string font;

private:
	GLuint vao;
	GLuint vbo;
	Shader shader;

	struct Glyph {
		GLuint data;
		FT_Pos next_glyph_offset;
		vec2 glyph_size;
		vec2 bearing_offset;
	};

	std::map<GLchar, Glyph> glyph_map;
};