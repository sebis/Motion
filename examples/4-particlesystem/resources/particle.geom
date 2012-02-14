/**
 * Copyright 2011-2012 Sebastian Eriksson.
 * This file is part of Motion.
 *
 * Motion is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Motion is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Motion. If not, see <http://www.gnu.org/licenses/>.
 */

#version 150
 
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float aspect;

in vec4 vertex_Color[];
in float vertex_Size[];

out vec4 pass_Color;
out vec2 pass_TexCoord;

void main() {

	vec2 viewportScale = vec2(0.5f / aspect, 0.5f);

	pass_Color = vertex_Color[0];
	float size = vertex_Size[0];

	if (size <= 0)
		return;

	gl_Position = gl_in[0].gl_Position;
	gl_Position.xy += vec2(-1, 1) * size * viewportScale;
	pass_TexCoord = vec2(0, 1);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.xy += vec2(-1, -1) * size * viewportScale;
	pass_TexCoord = vec2(0, 0);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.xy += vec2(1, 1) * size * viewportScale;
	pass_TexCoord = vec2(1, 1);
	EmitVertex();

	gl_Position = gl_in[0].gl_Position;
	gl_Position.xy += vec2(1, -1) * size * viewportScale;
	pass_TexCoord = vec2(1, 0);
	EmitVertex();

	EndPrimitive();
}