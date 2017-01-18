#version 450

in vec3 normal_out;
in vec2 uv_out;
in vec3 fragpos_out;

out vec4 colour;

uniform vec3 light_position;
uniform sampler2D tex;

void main() {
	vec3 normal = normalize(normal_out);
	vec3 light_direction = normalize(light_position - fragpos_out);

	float diffuse = max(dot(normal, light_direction), 0.0);
	float ambient = 0.2;

	vec2 uv = uv_out;
	colour = texture(tex, uv) * (diffuse + ambient);
}