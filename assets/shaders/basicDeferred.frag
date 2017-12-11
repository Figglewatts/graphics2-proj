#version 330 core

in vec2 vert_UV; 
in vec3 vert_Norm; 
in vec3 vert_WorldPos; 

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec4 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;

uniform vec3 ViewPos;

void main() 
{ 
    WorldPosOut = vert_WorldPos; 
    DiffuseOut.rgb = texture(tex1, vert_UV).rgb; 
	DiffuseOut.a = 0;
    NormalOut = normalize(vert_Norm); 
}