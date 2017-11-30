#version 330 core

uniform sampler2D gDiffuse;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform vec2 gScreenSize;

out vec4 FragColor;

struct Light
{
	vec3 position;
	vec4 diffuse;
	float intensity;
	float linearAttenuation;
	float expAttenuation;
};
uniform Light light;

vec2 calcTexCoord()
{
	return gl_FragCoord.xy / gScreenSize;
}

vec4 calcPointLight(Light pointLight, vec3 fragPos, vec3 fragDiffuse, vec3 fragNormal)
{
	vec3 lightDir = normalize(pointLight.position - fragPos);
	float diff = max(dot(fragNormal, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse.xyz * fragDiffuse * diff;

	float dist = length(pointLight.position - fragPos);
	float attenuation = 1.0 / (1.0 + light.linearAttenuation * pow(dist, 2));
	//float attenuation = 1.0 / (1.0 + light.linearAttenuation * dist + light.expAttenuation * (dist * dist));
	diffuse *= attenuation * pointLight.intensity;

	return vec4(diffuse, 1);
}

void main()
{	
	vec2 texCoord = calcTexCoord();
	vec3 worldPos = texture(gPosition, texCoord).xyz;
	vec3 diffuse = texture(gDiffuse, texCoord).xyz;
	vec3 normal = texture(gNormal, texCoord).xyz;
	normal = normalize(normal);
	FragColor = calcPointLight(light, worldPos, diffuse, normal);
}