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

uniform vec3 ViewPos;

vec2 calcTexCoord()
{
	return gl_FragCoord.xy / gScreenSize;
}

vec4 calcDirLight(vec3 fragNormal, vec4 fragDiffuse, float fragSpecular, vec3 fragPos, Light dirLight)
{
	// ambient
	float ambientStrength = 0.1;
	vec4 ambient = ambientStrength * dirLight.ambient * fragDiffuse;

	// diffuse
	float diff = max(dot(fragNormal, -dirLight.direction), 0.0);
	vec4 diffuse = diff * dirLight.diffuse * dirLight.intensity * fragDiffuse;

	// specular
	vec3 fragToEye = normalize(ViewPos - fragPos);
	vec3 reflection = normalize(reflect(dirLight.direction, fragNormal));
	float specularFactor = max(0.0, dot(fragToEye, reflection));
	specularFactor = pow(specularFactor, 32.0);
	vec4 specular = specularFactor * dirLight.diffuse * fragSpecular;

	//float nDotHalfVec = max(0.0, dot(viewDir, halfVec));
	//float spec = max(0.0, pow(nDotHalfVec, 16));

	//float spec = max(dot(viewDir, reflectDir), 0.0);
	//vec3 specular = fragSpecular * spec * dirLight.diffuse.rgb;

	return (ambient + diffuse + specular);
}

void main()
{	
	vec2 texCoord = calcTexCoord();
	vec3 worldPos = texture(gPosition, texCoord).rgb;
	vec3 diffuse = texture(gDiffuse, texCoord).rgb;
	vec3 normal = texture(gNormal, texCoord).rgb;
	float specular = texture(gDiffuse, texCoord).a;

	normal = normalize(normal);
	FragColor = calcDirLight(normal, vec4(diffuse, 1), specular, worldPos, light);
}