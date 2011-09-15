#version 150

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

in vec3 in_Position;
in vec3 in_Normal;
in vec4 in_Color;

out vec3 pass_Normal;
out vec4 pass_Color;

void main() {
    pass_Color = in_Color;
	pass_Normal = mat3(world) * in_Normal;
    gl_Position = projection * view * world * vec4(in_Position, 1.0f);
}