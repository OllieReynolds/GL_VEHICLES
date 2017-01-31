#include "..\include\ui.h"

UI::UI() { }

UI::UI(const Camera& camera) : index_active_button(-1), index_pressed_button(-1) {
	attributes_ui = std::vector<Button_Attributes>();
	std::string button_labels[7] = { "ADD", "REMOVE", "EDIT", "FOLLOW", "PLAY", "PAUSE", "NEW" };
	for (int i = 0; i < 7; i++) {
		float width_by_buttons = camera.resolution.x / 7;
		float p = (i * width_by_buttons) + (width_by_buttons * 0.5f);
		attributes_ui.push_back({ { p, 740.f },{ 162.f, 32.f }, utils::colour::black, button_labels[i] });
	}
}

void UI::update(const vec2& cursor_position, const bool mouse_pressed) {
	index_active_button = -1;
	index_pressed_button = -1;

	for (int i = 0; i < attributes_ui.size(); i++) {
		float l = attributes_ui[i].position.x - (attributes_ui[i].size.x * 0.5f);
		float r = attributes_ui[i].position.x + (attributes_ui[i].size.x * 0.5f);
		float u = attributes_ui[i].position.y + (attributes_ui[i].size.y * 0.5f);
		float d = attributes_ui[i].position.y - (attributes_ui[i].size.y * 0.5f);
		if (utils::point_quad_intersect(cursor_position, l, r, u, d)) index_active_button = i;
	}

	if (mouse_pressed && index_active_button != -1)
		index_pressed_button = index_active_button;
}