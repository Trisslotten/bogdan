#version 430

layout(location = 0) in vec3 vert_position;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_tex;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 normal;
out vec2 tex;

void main()
{
	normal = transpose(inverse(mat3(model)))*normalize(vert_normal);
	tex = vert_tex;
	gl_Position = projection * view * model * vec4(vert_position, 1.0);
}