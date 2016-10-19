#version 450

in vec2 uv;
out vec4 colour;

void main() {

	colour = vec4(uv, 0.0, 1.0);
}