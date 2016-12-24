#include "..\include\hud.h"

Hud::Hud() {
	button_attributes.push_back({{100.f, 700.f},  {192.f, 32.f}, {0.1f, 0.2f, 0.1f, 1.f}, "add vehicle"});
	button_attributes.push_back({{300.f, 700.f},  {192.f, 32.f}, {0.1f, 0.2f, 0.1f, 1.f}, "remove vehicle"});
	button_attributes.push_back({{500.f, 700.f},  {192.f, 32.f}, {0.1f, 0.2f, 0.1f, 1.f}, "follow vehicle"});
	button_attributes.push_back({{700.f, 700.f},  {192.f, 32.f}, {0.1f, 0.2f, 0.1f, 1.f}, "pause"});
	button_attributes.push_back({{900.f, 700.f},  {192.f, 32.f}, {0.1f, 0.2f, 0.1f, 1.f}, "play"});
	button_attributes.push_back({{1100.f, 700.f}, {192.f, 32.f}, {0.1f, 0.2f, 0.1f, 1.f}, "edit vehicle"});

	text_renderer = Text_Renderer{18, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
	active_button = -1;
	pressed_button = -1;
}

void Hud::init() {
	quad_renderer.init("C:/Users/Ollie/Desktop/debug.png");
	text_renderer.init({1366.f, 768.f});
}

void Hud::update(const vec2& cursor_position, bool mouse_pressed) {
	active_button = -1;
	pressed_button = -1;
	for (int i = 0; i < button_attributes.size(); i++) {
		float l = button_attributes[i].position.x - (button_attributes[i].size.x * 0.5f);
		float r = button_attributes[i].position.x + (button_attributes[i].size.x * 0.5f);
		float u = button_attributes[i].position.y + (button_attributes[i].size.y * 0.5f);
		float d = button_attributes[i].position.y - (button_attributes[i].size.y * 0.5f);
		if (utils::point_quad_intersect(cursor_position, l, r, u, d)) {
			active_button = i;
		}
	}

	if (mouse_pressed && active_button != -1) {
		pressed_button = active_button;
		button_actions();
	}
}


void Hud::button_actions() {
	switch (pressed_button) {
	case 0:
		break;

	default:

		break;
	}
}

void Hud::draw(const mat4& projection_matrix, const mat4& view_matrix) {
	for (ButtonAttributes attrib : button_attributes) {
		quad_renderer.draw_2D(view_matrix, projection_matrix, attrib.position, attrib.size, attrib.colour);
		text_renderer.draw(attrib.label, attrib.position, true);
	}

	if (active_button != -1) {
		text_renderer.draw("wow", button_attributes[active_button].position, true);
	}

	if (pressed_button != -1) {
		vec2 position = {button_attributes[active_button].position.x, button_attributes[active_button].position.y - 50};
		text_renderer.draw("go", position, true);
	}
}

void Hud::destroy() {
	quad_renderer.destroy();
	text_renderer.destroy();
}