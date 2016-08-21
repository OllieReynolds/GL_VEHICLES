#version 450

in vec2 uv_coords;

out vec4 frag_colour;

uniform vec4 colour;

void main() {
	float d = distance(uv_coords, vec2(0.5, 0.5));

	vec4 c = colour;
	c.a *= pow((1.0 - (2.0 * d)), 1.0);


	frag_colour = c;
};