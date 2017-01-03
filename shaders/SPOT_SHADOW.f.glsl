#version 450

in vec2 uv_out;
out vec4 colour;

void main() {
	float d = distance(vec2(0.5, 0.5), uv_out);
	vec4 c = vec4(0.f, 0.f, 0.f, 1.f);

	c.a *= pow(1.0 - (2.0 * d), 0.7);

	if (d > 0.5)
		discard;
	else
		colour = c;
}