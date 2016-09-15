#version 450

layout(std140) uniform Obstacles {
	vec4 colour;
	vec2 position;
	vec2 size;
} obstacle;


void main() {
	gl_Position = vec4(obstacle.position, 0.0, 1.0);
};