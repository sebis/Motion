#version 150
#extension GL_ARB_gpu_shader5 : enable

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 light_Position;

in vec3 in_Position;
in vec3 in_Normal;
in vec4 in_Color;
in vec2 in_Texcoord;

out vec3 pass_lightDir;
out float pass_lightDist;

out vec3 pass_Position;
out vec3 pass_Normal;
out vec3 pass_Tangent;
out vec3 pass_Bitangent;

out vec2 pass_Texcoord;
out vec4 pass_Color;

void main(void)
{
	// Texture calculations
	pass_Texcoord = in_Texcoord;
	
	// Position in view space
	vec4 position = view * world * vec4(in_Position, 1.0);
	gl_Position = projection * position;
	pass_Position = position.xyz;

	// Normal calculations (use inverse transpose of world matrix)
	mat3 modelView = mat3(view) * transpose(inverse(mat3(world)));
	pass_Normal = normalize(modelView * in_Normal);

	// Calculate cam space tangent and bitangent used in normal mapping
	// Define tangent perpendicular to y-axis
	pass_Tangent = normalize(modelView * cross(vec3(0.0, 1.0, 0.0), in_Normal));
	pass_Bitangent = normalize(cross(pass_Normal, pass_Tangent));
	
	if (light_Position.w == 0.0) {
		// treat light as directional if w = 0
		pass_lightDir = normalize(mat3(view) * light_Position.xyz);
		// disable attenuation
		pass_lightDist = 0.0;
	} else {
		// treat light with a position and transform it to view space
		vec3 lightDir = pass_Position - vec3(view * light_Position);
		pass_lightDir = normalize(lightDir);
		pass_lightDist = length(lightDir);
	}
	
	pass_Color = in_Color;
}
