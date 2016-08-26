#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 uv_coords;

uniform float size;
uniform mat4 projection;

void main() {
	const vec4 position = gl_in[0].gl_Position;

	gl_Position = projection * (position + vec4(-size, -size, 0, 0));
	uv_coords = vec2(0, 0);
	EmitVertex();

	gl_Position = projection * (position + vec4(-size, size, 0, 0));
	uv_coords = vec2(0, 1);
	EmitVertex();

	gl_Position = projection * (position + vec4(size, -size, 0, 0));
	uv_coords = vec2(1, 0);
	EmitVertex();

	gl_Position = projection * (position + vec4(size, size, 0, 0));
	uv_coords = vec2(1, 1);
	EmitVertex();

	EndPrimitive();
}