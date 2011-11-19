#version 150
 
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec4 vertex_Color[];

out vec4 pass_Color;
out vec2 pass_TexCoord;

void main() {

	pass_Color = vertex_Color[0];

	gl_Position = gl_in[0].gl_Position;
	pass_Color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	pass_TexCoord = vec2(0, 1);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(0.0f, -5.0f, 0.0f, 0.0f);
	pass_Color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	pass_TexCoord = vec2(0, 0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(5.0f, 0.0f, 0.0f, 0.0f);
	pass_Color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	pass_TexCoord = vec2(1, 1);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(5.0f, -5.0f, 0.0f, 0.0f);
	pass_Color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	pass_TexCoord = vec2(1, 0);
	EmitVertex();

	EndPrimitive();
}