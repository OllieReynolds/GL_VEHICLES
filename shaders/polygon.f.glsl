#version 450

in vec2 uv_coords;

out vec4 frag_colour;

layout(std140) uniform Sectors{
	vec4 colour;
	vec2 position;
	vec2 size;
	float rotation;
	float theta;
	bool is_body;
} sector;

bool point_segment_intersect(vec2 p, vec2 a, vec2 o, vec2 b) {
	return distance(o, p) <= length(a - o) 
		&& ((p.x - o.x) * (a.y - o.y)) - ((p.y - o.y) * (a.x - o.x)) >= 0 
		&& ((p.x - o.x) * (b.y - o.y)) - ((p.y - o.y) * (b.x - o.x)) <= 0;
}

void main() {
	vec2 uv_centerpoint = vec2(0.5, 0.5);
	float d = distance(uv_coords, uv_centerpoint);
	vec4 c = sector.colour;

	if (!sector.is_body) {
		////////////////////////////////////////////////////////////////////////////
		// Point segment intersect
		////////////////////////////////////////////////////////////////////////////
		float th  = clamp(sector.theta,1.0, 180.0) * 0.50;
		float rot = sector.rotation + 90.0;

		float rA = radians(rot + th);
		vec2 vA = (vec2(cos(rA), sin(rA)) + vec2(1.0, 1.0)) * 0.5;

		float rB = radians(rot - th);
		vec2 vB = (vec2(cos(rB), sin(rB)) + vec2(1.0, 1.0)) * 0.5;

		if (!point_segment_intersect(uv_coords, vA, uv_centerpoint, vB)) 
			discard;

		////////////////////////////////////////////////////////////////////////////
		// Circular distance cutoff
		///////////////////////////////////////////////////////////////////////////
		c.a *= pow(1.0 - (2.0 * d), 0.1);
	}

	////////////////////////////////////////////////////////////////////////////
	// Paint
	////////////////////////////////////////////////////////////////////////////
	frag_colour = c;
};