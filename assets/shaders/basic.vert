#version 330 core

layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec3 in_Norm;
layout (location = 2) in vec2 in_UV;
layout (location = 3) in vec3 in_Color;
uniform mat4 u_MVP;

out vec4 out_Color;

void main()
{
	gl_Position = u_MVP * vec4(aPos.x, aPos.y, aPos.z, 1);
}