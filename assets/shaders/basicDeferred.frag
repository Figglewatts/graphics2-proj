#version 330 core

in vec2 vert_UV; 
in vec3 vert_Norm; 
in vec3 vert_WorldPos; 

layout (location = 0) out vec3 WorldPosOut; 
layout (location = 1) out vec3 DiffuseOut; 
layout (location = 2) out vec3 NormalOut; 

uniform sampler2D gColorMap;

void main() 
{ 
    WorldPosOut = vert_WorldPos; 
    DiffuseOut = texture(gColorMap, vert_UV).xyz; 
    NormalOut = normalize(vert_Norm); 
}