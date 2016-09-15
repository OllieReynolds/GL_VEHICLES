#version 450

layout(std140) uniform Sectors{
	vec4 colour;
	vec2 position;
	vec2 start;
	vec2 end;
	float radius;
} sector;


void main() {
	gl_Position = vec4(sector.position, 0.0, 1.0);
};