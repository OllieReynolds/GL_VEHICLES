#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv_coord;

out vec2 uv;

uniform mat4 model;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * vec4(position, 0.0, 1.0);
	uv = uv_coord;
};