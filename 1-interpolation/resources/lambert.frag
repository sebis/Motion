#version 150

uniform vec3 lightDirection;

in vec3 pass_Normal;
in vec4 pass_Color;

out vec4 out_Color;

void main() {
	vec3 normal = normalize(pass_Normal);
	vec3 light = normalize(lightDirection);

	float ambient = 0.2f;
	float diffuse = max(0, dot(light, normal));

    out_Color = (ambient + diffuse) * pass_Color;
}