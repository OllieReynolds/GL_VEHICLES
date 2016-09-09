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
		float th  = sector.theta * 0.5;
		float rot = sector.rotation;

		float rA = radians(rot + th);
		vec2 vA = vec2(
			sector.size.x * cos(rA), 
			sector.size.y * sin(rA)
		);
		

		float rB = radians(rot - th);
		vec2 vB = vec2(
			sector.size.x * cos(rB), 
			sector.size.y * sin(rB)
		);


		if (!point_segment_intersect(uv_coords, vA, uv_centerpoint, vB)) 
			discard;

		////////////////////////////////////////////////////////////////////////////
		// Circular distance cutoff
		////////////////////////////////////////////////////////////////////////////
		c.a *= pow(1.0 - (2.0 * d), 0.98);
	}

	////////////////////////////////////////////////////////////////////////////
	// Paint
	////////////////////////////////////////////////////////////////////////////
	frag_colour = c;
};