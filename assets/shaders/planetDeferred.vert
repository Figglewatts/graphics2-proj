#version 330 core

layout (location = 0) in vec3 in_Pos;
layout (location = 1) in vec3 in_Norm;
layout (location = 2) in vec2 in_UV;
layout (location = 3) in vec4 in_Color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 vert_WorldPos;
out vec3 vert_Norm;
out vec2 vert_UV;

void main()
{
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Pos, 1.0);
	vert_Norm = mat3(ModelMatrix) * in_Norm;
	vert_UV = in_UV;
	vert_WorldPos = (ModelMatrix * vec4(in_Pos, 1.0)).xyz;
}