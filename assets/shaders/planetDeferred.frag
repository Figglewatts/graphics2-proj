#version 330 core

in vec2 vert_UV; 
in vec3 vert_Norm; 
in vec3 vert_WorldPos; 

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec4 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 

uniform sampler2D tex1;
uniform sampler2D tex2;

void main() 
{ 
    WorldPosOut = vert_WorldPos; 
    DiffuseOut.rgb = texture(tex1, vert_UV).rgb;
	DiffuseOut.a = texture(tex2, vert_UV).r;
    NormalOut = normalize(vert_Norm); 
}