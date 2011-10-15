#version 150

uniform vec3 lightDirection;

in vec3 pass_Position;
in vec3 pass_Normal;
in vec4 pass_Color;

out vec4 out_Color;

vec4 diffuseColor = vec4(0.75f, 0.75f, 0.75f, 1.0f);
vec4 ambientColor = vec4(0.05f, 0.05f, 0.05f, 1.0f);
vec4 specularColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
float shininess = 30.0f;

void main() {
	vec3 normal = normalize(pass_Normal);
	vec3 light = normalize(lightDirection);

	float ambient = 0.2f;
	float diffuse = max(0, dot(light, normal));
	float specular = 0.0f;

	if (diffuse > 0)
	{
		vec3 V = normalize(pass_Position);
		vec3 R = reflect(light, normal);
		specular = pow(max(dot(R, V), 0), shininess);
	}

    out_Color = ambient * ambientColor + diffuse * diffuseColor + specular * specularColor;
	//out_Color = (ambient + diffuse + specular) * pass_Color;
}