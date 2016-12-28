#version 450

in vec2 uv_out;
out vec4 colour;

uniform vec4 uniform_colour;
uniform bool draw_filled;

void main() {
	float d = distance(vec2(0.5, 0.5), uv_out);

	if (d > 0.5 || (!draw_filled && d < 0.4))
		discard;
	else
		colour = uniform_colour;
}