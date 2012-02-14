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

uniform vec3 lightDirection;

in vec3 pass_Position;
in vec3 pass_Normal;
in vec4 pass_Color;

out vec4 out_Color;

vec4 diffuseColor = vec4(0.75f, 0.75f, 0.75f, 1.0f);
vec4 ambientColor = vec4(0.05f, 0.05f, 0.05f, 1.0f);
vec4 specularColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
float shininess = 30.0f;

void main() {
	vec3 normal = normalize(pass_Normal);
	vec3 light = normalize(lightDirection);

	float ambient = 0.2f;
	float diffuse = max(0, dot(light, normal));
	float specular = 0.0f;

	if (diffuse > 0)
	{
		vec3 V = normalize(pass_Position);
		vec3 R = reflect(light, normal);
		specular = pow(max(dot(R, V), 0), shininess);
	}

    //out_Color = ambient * ambientColor + diffuse * pass_Color + specular * specularColor;
	out_Color = (ambient + diffuse + specular) * pass_Color;
}