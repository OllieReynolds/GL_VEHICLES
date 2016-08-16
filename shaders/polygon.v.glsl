#version 450

uniform vec2 vp;
uniform float scale;

out float v_scale;

void main() {
	gl_Position = vec4(vp, 0.0, 1.0);
	v_scale = scale;
};