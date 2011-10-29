#version 150

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_Position;
in vec4 in_Color;
in float in_Size;

out vec4 pass_Color;

void main() {
	pass_Color = in_Color;
	gl_PointSize = in_Size;
    gl_Position = projection * view * world * vec4(in_Position, 1.0f);
}