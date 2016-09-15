#version 450

in vec2 uv_coords;
out vec4 frag_colour;

layout(std140) uniform Sectors{
    vec4 colour;
    vec2 position;
    vec2 start;
    vec2 end;
    float radius;
} sector;


bool check_clockwise(vec2 v1, vec2 v2) {
    return (-v1.x * v2.y) + (v1.y * v2.x) > 0;
}

bool point_segment_intersect1(vec2 p, vec2 end, vec2 o, vec2 start) {
    return distance(o, p) <= sector.radius
        && !check_clockwise(start, p - o)
        && check_clockwise(end, p - o);
}

bool point_segment_intersect(vec2 p, vec2 a, vec2 o, vec2 b) {
    return distance(o, p) <= length(a - o) 
        && ((p.x - o.x) * (a.y - o.y)) - ((p.y - o.y) * (a.x - o.x)) >= 0 
        && ((p.x - o.x) * (b.y - o.y)) - ((p.y - o.y) * (b.x - o.x)) <= 0;
}

void main() {
    ////////////////////////////////////////////////////////////////////////////
    // Circle sector intersect
    ////////////////////////////////////////////////////////////////////////////
    vec2 uv_centerpoint = vec2(0.5, 0.5);
    if (!point_segment_intersect1(uv_coords, sector.end, uv_centerpoint, sector.start)) 
        discard;

    ////////////////////////////////////////////////////////////////////////////
    // Circular distance cutoff
    ////////////////////////////////////////////////////////////////////////////
    vec4 c = sector.colour;
    float d = distance(uv_coords, uv_centerpoint);
    c.a *= pow(1.0 - (2.0 * d), 1.0);

    ////////////////////////////////////////////////////////////////////////////
    // Paint
    ////////////////////////////////////////////////////////////////////////////
    frag_colour = c;
};