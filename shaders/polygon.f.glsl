#version 450

in vec2 uv_coords;

out vec4 frag_colour;

uniform vec4 colour;

void main() {
	float d = distance(uv_coords, vec2(0.5, 0.5));
	const float vec2 sector_start = {1.0, 0.25};
	const float vec2 sector_end = {0.25, 1.0};

	vec4 c = colour;
	c.a *= pow((1.0 - (2.0 * d)), 0.5);


	frag_colour = c;
};