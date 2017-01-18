#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec3 normal_out;
out vec3 fragpos_out;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0);

	// Generate normal matrix, as using non-uniform scale
	normal_out = mat3(transpose(inverse(model))) * normal;
	
	fragpos_out = vec3(model * vec4(position, 1.0));
};