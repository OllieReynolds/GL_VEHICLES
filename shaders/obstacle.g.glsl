#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

layout(std140) uniform Obstacles {
	vec4 colour;
	vec2 position;
	vec2 size;
} obstacle;

uniform mat4 projection;

void main() {
	const vec4 position = gl_in[0].gl_Position;

	vec2 span = obstacle.size * 0.5;

	gl_Position = projection * (position + vec4(-span.x, -span.y, 0, 0));
	EmitVertex();

	gl_Position = projection * (position + vec4(-span.x, span.y, 0, 0));
	EmitVertex();

	gl_Position = projection * (position + vec4(span.x, -span.y, 0, 0));
	EmitVertex();

	gl_Position = projection * (position + vec4(span.x, span.y, 0, 0));
	EmitVertex();

	EndPrimitive();
}