#pragma once

#include "quad_renderer.h"
#include "text_renderer.h"

struct ButtonAttributes {
	vec2 position;
	vec2 size;
	vec4 colour;
	std::string label;
};

class Hud {
public:
	Hud();

	void init();
	void update(const vec2& cursor_position, bool mouse_pressed);
	void draw(const mat4& projection_matrix, const mat4& view_matrix);
	void destroy();

	int active_button;
	int pressed_button;

private:
	void button_actions();
	
	std::vector<ButtonAttributes> button_attributes;

	Quad_Renderer quad_renderer;
	Text_Renderer text_renderer;
};