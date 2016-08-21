#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

in vec2 v_scale[];

out vec2 uv_coords;

void main() {
	const vec4 position = gl_in[0].gl_Position;
	const vec2 scale = v_scale[0];


	gl_Position = projection * (position + vec4(-scale.x, -scale.y, 0, 0));
	uv_coords = vec2(0, 0);
	EmitVertex();

	gl_Position = projection * (position + vec4(-scale.x, scale.y, 0, 0));
	uv_coords = vec2(0, 1);
	EmitVertex();

	gl_Position = projection * (position + vec4(scale.x, -scale.y, 0, 0));
	uv_coords = vec2(1, 0);
	EmitVertex();

	gl_Position = projection * (position + vec4(scale.x, scale.y, 0, 0));
	uv_coords = vec2(1, 1);
	EmitVertex();

	EndPrimitive();
}