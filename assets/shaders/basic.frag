#version 330 core

in vec4 vert_Color;
in vec4 vert_Normal;
in vec2 vert_UV;
out vec4 diffuseColor;

uniform sampler2D diffuse_Tex;

void main()
{
	diffuseColor = texture(diffuse_Tex, vert_UV);
}