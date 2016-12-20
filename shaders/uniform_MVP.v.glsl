#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 uv_out;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	uv_out = uv;
};