#pragma once

#include "quad_renderer.h"
#include "text.h"

struct ButtonAttributes {
	vec2 position;
	vec2 size;
	vec4 colour;
	std::string label;
};

class Hud {
public:
	Hud();

	void init(const mat4& projection_matrix);
	void draw(const mat4& projection_matrix, const mat4& view_matrix);
	void destroy();

private:
	std::vector<ButtonAttributes> button_attributes;

	Quad_Renderer quad_renderer;
	Text text_renderer;
};