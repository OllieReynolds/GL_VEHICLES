#version 450

in vec2 uv_coords;

out vec4 frag_colour;

void main() {
	float d = distance(uv_coords, vec2(0.5, 0.5));
	
	vec4 c = vec4(1.0, 0.0, 0.0, 1.0);

	c.a *= pow((1.0 - (2.0 * d)), 0.5);


	frag_colour = c;
};