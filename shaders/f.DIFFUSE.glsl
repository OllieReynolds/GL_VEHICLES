#version 450

in vec3 normal_out;
in vec3 fragpos_out;

out vec4 colour;

uniform vec4 uniform_colour;
uniform vec3 light_position;

void main() {
	vec3 normal = normalize(normal_out);
	vec3 light_direction = normalize(light_position - fragpos_out);
	float diffuse = max(dot(normal, light_direction), 0.0);

	float ambient = 0.2;

	colour = uniform_colour * (diffuse + ambient);
}