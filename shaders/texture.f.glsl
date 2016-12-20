#version 450

in vec2 uv_out;

out vec4 colour;

uniform sampler2D tex;

void main() {
	colour = texture(tex, uv_out);
}