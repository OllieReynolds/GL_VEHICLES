#pragma once

#include "camera.h"
#include "maths.h"
#include "utils.h"

#include <vector>

using namespace maths;
using namespace utils;

struct Button_Attributes {
	vec2 position;
	vec2 size;
	vec4 colour;
	std::string label;
};

struct UI {
	UI();
	UI(const Camera& camera);

	void update(const vec2& cursor_position, const bool mouse_pressed);

	int index_active_button;
	int index_pressed_button;
	std::vector<Button_Attributes> attributes_ui;
};