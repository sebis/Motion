#version 150

uniform sampler2D sampler0;

in vec2 pass_TexCoord;
in vec4 pass_Color;

out vec4 out_Color;

void main() {
	out_Color = pass_Color * texture2D(sampler0, pass_TexCoord);
	out_Color.a = 1.0f;
}