#version 150

uniform sampler2D sampler0;
uniform sampler2D normalMap;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

uniform float shininess;
uniform bool useTexture;
uniform bool useNormalMap;

in vec3 pass_lightDir;
in float pass_lightDist;

in vec3 pass_Position;
in vec3 pass_Normal;
in vec3 pass_Tangent;
in vec3 pass_Bitangent;

in vec2 pass_Texcoord;
in vec4 pass_Color;

out vec4 fragment_Color;

void main(void)
{
	// start with ambient as "base" color
	vec4 lightColor = ambientColor;
	
	// re-normalize interpolated unit vectors
	vec3 lightDir = normalize(pass_lightDir);
	vec3 normal = normalize(pass_Normal);

	if (useNormalMap)
	{
		// Normal from the map in tangent space
		vec3 normalTan = normalize(texture2D(normalMap, pass_Texcoord).xyz * 2.0 - 1.0);

		// re-normalize
		vec3 tangent = normalize(pass_Tangent);
		vec3 bitangent = normalize(pass_Bitangent);

		// build final normal from components
		normal = normalTan.x*tangent + normalTan.y*bitangent + normalTan.z*normal;
	}
	
	float attenuation = 1.0;
	// calculate 1/r^2 attenuation if we don't have a directional light
	if (pass_lightDist > 0) {
		attenuation = 1.0 / max(1.0, pow(pass_lightDist, 2));
	}

	// Calculate diffuse light
	float cosTerm = max(dot(lightDir, normal), 0);
	lightColor += (cosTerm * diffuseColor);

	// Calculate specular highlights
	if (cosTerm > 0.0) // Only to surfaces facing the light
	{
		vec3 viewDir = normalize(-pass_Position);
		vec3 reflectDir = reflect(-lightDir, normal);
		float phong = pow(max(dot(reflectDir, viewDir), 0), shininess);
		lightColor += (phong * specularColor) * attenuation;
	}
	
	// use simple material model
	vec4 materialColor = pass_Color;
	
	if (useTexture) {
		materialColor *= texture2D(sampler0, pass_Texcoord);
	}
	
	fragment_Color = materialColor * lightColor;
}