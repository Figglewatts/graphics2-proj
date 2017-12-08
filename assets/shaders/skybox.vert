#version 330 core
layout (location = 0) in vec3 in_Pos;

out vec3 TexCoords;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
    TexCoords = in_Pos;
    vec4 pos = ProjectionMatrix * ViewMatrix * vec4(in_Pos, 1.0);
    gl_Position = pos.xyww;
}  