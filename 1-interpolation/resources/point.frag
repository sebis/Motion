#version 150

in vec4 pass_Color;

out vec4 out_Color;

void main() {
	vec2 n = gl_PointCoord * 2.0f - 1.0f;
	if(length(n) > 1.0) 
		discard;

	out_Color = pass_Color;

	//vec3 N = vec3(n, sqrt(1.0 - length(n)));
	//float diffuse = max(0.0, dot(lightDir, N));
	//out_Color *= diffuse;
}