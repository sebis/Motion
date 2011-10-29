#version 150

vec3 lightDirection = vec3(1.0f, 0.5f, 0.25f);
vec4 diffuseColor = vec4(0.75f, 0.75f, 0.75f, 1.0f);
vec4 ambientColor = vec4(0.05f, 0.05f, 0.05f, 1.0f);
vec4 specularColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
float shininess = 30.0f;

uniform sampler2D sampler0;

in vec3 pass_Position;
in vec3 pass_Normal;
in vec2 pass_TexCoord;
in vec4 pass_Color;

out vec4 out_Color;

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

    //out_Color = ambient * ambientColor + diffuse * pass_Color + specular * specularColor;
	out_Color = (ambient + diffuse + specular) * pass_Color * texture2D(sampler0, pass_TexCoord);
}