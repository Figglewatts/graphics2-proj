#version 330 core

in vec4 vert_Color;
in vec4 vert_Normal;
in vec2 vert_UV;
out vec4 diffuseColor;

void main()
{
	diffuseColor = vec4(1.0, 1.0, 1.0, 1.0);
}