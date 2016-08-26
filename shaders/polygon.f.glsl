#version 450

in vec2 uv_coords;

out vec4 frag_colour;

uniform vec4 colour;

bool check_clockwise(vec2 a, vec2 b, vec2 p) {
	float d = ((p.x - a.x) * (b.y - a.y)) - ((p.y - a.y) * (b.x - a.x));
	return d > 0;
}

bool point_circle_intersect(vec2 p, vec2 o, float r) {
	return distance(o, p) < r;
}

bool point_segment_intersect(vec2 p, vec2 a, vec2 o, vec2 b) {
	float r = length(a - o);

	return
		point_circle_intersect(p, o, r) &&
		check_clockwise(o, a, p) && !check_clockwise(o, b, p);
}

void main() {
	if (!point_segment_intersect(
		uv_coords, vec2(0.0, 1.0), vec2(0.5, 0.5), vec2(1.0, 1.0))
	) discard;

	vec4 c = colour;

	float d = distance(uv_coords, vec2(0.5, 0.5));
	c.a *= pow(1.0 - (2.0 * d), 0.5);

	frag_colour = c;
};