#version 150

uniform sampler2D sampler0;

in vec4 pass_Color;
in vec2 pass_TexCoord;

out vec4 out_Color;

void main() {
	out_Color = texture2D(sampler0, pass_TexCoord) * pass_Color;
}