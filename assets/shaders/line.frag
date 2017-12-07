#version 330 core

uniform vec4 LineColor;

out vec4 diffuseColor;

void main()
{
	diffuseColor = LineColor;
}