#version 450

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 projection;

in float v_scale[];

void main() {
	const vec4 position = gl_in[0].gl_Position;
	const float scale = v_scale[0];


	gl_Position = projection * (position + vec4(-scale, -scale, 0, 0));
	EmitVertex();

	gl_Position = projection * (position + vec4(-scale, scale, 0, 0));
	EmitVertex();

	gl_Position = projection * (position + vec4(scale, -scale, 0, 0));
	EmitVertex();

	gl_Position = projection * (position + vec4(scale, scale, 0, 0));
	EmitVertex();

	EndPrimitive();
}