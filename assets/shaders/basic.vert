#version 330 core

layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec3 in_Norm;
layout (location = 2) in vec2 in_UV;
layout (location = 3) in vec4 in_Color;
uniform mat4 u_MVP;

out vec4 vert_Color;
out vec4 vert_Norm;
out vec2 vert_UV;

void main()
{
	gl_Position = vec4(in_Pos, 1.0);
	vert_Norm = vec4(in_Norm, 1.0);
	vert_UV = in_UV;
	vert_Color = in_Color;
}