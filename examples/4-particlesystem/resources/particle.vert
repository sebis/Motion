#version 150

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_Position;
in vec4 in_Color;
in float in_Size;

out vec4 vertex_Color;
out float vertex_Size;

void main() {
	vertex_Color = in_Color;
	vertex_Size = in_Size;
    gl_Position = projection * view * world * vec4(in_Position, 1.0f);
}