#version 150

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_Position;
in vec4 in_Color;
in float in_Size;

out vec4 vertex_Color;

void main() {
	vertex_Color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//gl_PointSize = 10.0f;
    gl_Position = projection * view * world * vec4(in_Position, 1.0f);
}