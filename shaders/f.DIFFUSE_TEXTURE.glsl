#version 450

in vec3 normal_out;
in vec2 uv_out;
in vec3 fragpos_out;

out vec4 colour;

struct Light {
	vec3 position;
	vec3 colour;
	float intensity;
};

uniform int num_lights;
uniform Light lights[200];

uniform sampler2D tex;

vec3 calc_lighting(vec3 normal, Light light) {
	float constant = 0.1;
    float linear = 0.09;
    float quadratic = 0.032;

	float intensity = 5.0;

	float distance = length(light.position - fragpos_out);
	float attenuation = intensity / (constant + (linear * distance) + (quadratic * (distance)));

	vec3 light_direction = normalize(light.position - fragpos_out);
	float diffuse = max(dot(normal, light_direction), 0.0) * attenuation;
	return light.colour * diffuse * vec3(texture(tex, uv_out)) * light.intensity;
}

void main() {
	vec3 normal = normalize(normal_out);

	vec3 result = vec3(0.0);
	for (int i = 0; i < num_lights; i++) {
		result += calc_lighting(normal, lights[i]);
	}

	result += vec3(0.2, 0.2, 0.2) * vec3(texture(tex, uv_out));

	//vec2 uv = uv_out;
	colour = vec4(result, 1.0);
}