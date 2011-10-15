#version 150

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_Position;
in vec2 in_TexCoord;
in vec4 in_Color;

out vec2 pass_TexCoord;
out vec4 pass_Color;

void main() {
	pass_TexCoord = in_TexCoord;
	pass_Color = in_Color;

    gl_Position = projection * view * world * vec4(in_Position, 1.0f);
}