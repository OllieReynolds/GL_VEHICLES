#include "..\include\hud.h"

Hud::Hud() {
	button_attributes.push_back({{300.f, 300.f},  {100.f, 64.f}, {0.2f, 0.2f, 0.2f, 1.f}, "test"});
	button_attributes.push_back({{450.f, 300.f},  {100.f, 64.f}, {0.2f, 0.2f, 0.2f, 1.f}, "test"});
	button_attributes.push_back({{600.f, 300.f},  {100.f, 64.f}, {0.2f, 0.2f, 0.2f, 1.f}, "test"});
	button_attributes.push_back({{750.f, 300.f},  {100.f, 64.f}, {0.2f, 0.2f, 0.2f, 1.f}, "test"});
	button_attributes.push_back({{900.f, 300.f},  {100.f, 64.f}, {0.2f, 0.2f, 0.2f, 1.f}, "test"});
	button_attributes.push_back({{1050.f, 300.f}, {100.f, 64.f}, {0.2f, 0.2f, 0.2f, 1.f}, "test"});

	text_renderer = Text{18, "data/ShareTechMono-Regular.ttf", vec4{1.f, 1.f, 1.f, 1.f}};
}

void Hud::init(const mat4& projection_matrix) {
	quad_renderer.init_quad(projection_matrix);
	text_renderer.init_text({1366.f, 768.f});
}

void Hud::draw(const mat4& projection_matrix, const mat4& view_matrix) {
	for (ButtonAttributes attrib : button_attributes) {
		quad_renderer.draw_quad_2D(view_matrix, projection_matrix, attrib.position, attrib.size, attrib.colour);
		text_renderer.draw_text(attrib.label, attrib.position, true);
	}
}

void Hud::destroy() {
	quad_renderer.destroy_quad();
	text_renderer.destroy_text();
}