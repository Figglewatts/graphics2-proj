#version 330 core

uniform sampler2D gDiffuse;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform vec2 gScreenSize;

out vec4 FragColor;

struct Light
{
	vec3 direction;
	vec4 diffuse;
	vec4 ambient;
	float intensity;
};
uniform Light light;

vec2 calcTexCoord()
{
	return gl_FragCoord.xy / gScreenSize;
}

vec4 calcDirLight(vec3 fragNormal, vec4 fragDiffuse, Light dirLight)
{
	float NdotL;
	vec3 direction = -normalize(dirLight.direction);
	NdotL = max(dot(fragNormal, direction), 0.0);
	vec4 diffuseWithFactor = dirLight.diffuse * NdotL;
	return (diffuseWithFactor + (dirLight.ambient * 0.1)) * fragDiffuse * dirLight.intensity;
}

void main()
{	
	vec2 texCoord = calcTexCoord();
	vec3 worldPos = texture(gPosition, texCoord).xyz;
	vec3 diffuse = texture(gDiffuse, texCoord).xyz;
	vec3 normal = texture(gNormal, texCoord).xyz;
	normal = normalize(normal);
	FragColor = calcDirLight(normal, vec4(diffuse, 1), light);
}