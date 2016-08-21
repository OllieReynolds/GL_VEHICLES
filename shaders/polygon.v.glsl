#version 450

uniform vec2 vp;
uniform vec2 scale;

out vec2 v_scale;

void main() {
	gl_Position = vec4(vp, 0.0, 1.0);
	v_scale = scale;
};