#version 430

out vec4 frag_color;

in vec3 normal;
in vec2 tex;

void main() 
{
	vec3 n = normalize(normal);
	frag_color = vec4(tex, 0.0, 1.0);
}