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
    
	vec3 distVec = vert_WorldPos - ViewPos;
	float len = length(distVec);

	float minDist = 100;
	float maxDist = 1000;
	float slope = 1 / (maxDist - minDist);
	float intercept = 1 - maxDist * slope;
	float drawRatio = 1 - clamp(1/(maxDist - minDist) * len + intercept, 0, 1);

	float uvScale = 100;
	vec4 detailCol = texture(tex3, vert_UV * uvScale) * drawRatio;

	DiffuseOut.a = texture(tex2, vert_UV).r;

	vec4 detailMapped = (1 - DiffuseOut.a) * detailCol * 0.5f;

	DiffuseOut.rgb = texture(tex1, vert_UV).rgb + detailMapped.rgb;

    NormalOut = normalize(vert_Norm); 
}