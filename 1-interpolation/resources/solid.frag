#version 150

uniform vec3 lightDirection;
uniform vec3 eyePosition;

in vec3 pass_Position;
in vec3 pass_Normal;
in vec4 pass_Color;

out vec4 out_Color;

void main() {
	vec3 normal = normalize(pass_Normal);
	vec3 light = normalize(lightDirection);
	vec3 eye = normalize(pass_Position - eyePosition);

	float ambient = 0.2f;
	float diffuse = max(0, dot(light, normal));
	float specular = 0.0f;
	if (diffuse > 0)
		specular = pow(max(0, dot(reflect(light, normal), eye)), 5.0f);

    out_Color = (ambient + diffuse) * pass_Color + specular * vec4(0.5f);
}