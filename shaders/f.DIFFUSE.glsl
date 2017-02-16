#version 450

in vec3 normal_out;
in vec3 fragpos_out;

out vec4 colour;

uniform vec4 uniform_colour;

struct Light {
	vec3 position;
	vec3 colour;
	float intensity;
};

uniform int num_lights;
uniform Light lights[200];

vec3 calc_lighting(vec3 normal, Light light) {
	vec3 light_direction = normalize(light.position - fragpos_out);
	float diffuse = max(dot(normal, light_direction), 0.0);
	return light.colour * diffuse * uniform_colour.xyz * light.intensity;
}

void main() {
	vec3 normal = normalize(normal_out);

	Light light = Light(vec3(0.0, 30.0, 0.0), vec3(1.0, 1.0, 1.0), 1.0);
	vec3 result = calc_lighting(normal, light);

	result += vec3(0.2, 0.2, 0.2) * uniform_colour.xyz;

	colour = vec4(result, 1.0);
}