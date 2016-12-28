#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 normal_out;
out vec2 uv_out;
out vec3 fragpos_out;


void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);
	normal_out = mat3(transpose(inverse(model))) * normal;
	uv_out = uv;
	fragpos_out = vec3(model * vec4(position, 1.0));
};