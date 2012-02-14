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

vec3 lightDirection = vec3(1.0f, 0.5f, 0.25f);

in vec3 pass_Normal;
in vec4 pass_Color;

out vec4 out_Color;

void main() {
	vec3 normal = normalize(pass_Normal);
	vec3 light = normalize(lightDirection);

	float ambient = 0.2f;
	float diffuse = max(0, dot(light, normal));

    out_Color = (ambient + diffuse) * pass_Color;
}