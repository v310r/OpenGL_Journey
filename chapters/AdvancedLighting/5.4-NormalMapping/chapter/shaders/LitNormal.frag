#version 460 core

out vec4 FragColor;

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	float intensity;
};


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform sampler2D specularMap;

uniform DirectionalLight directionalLight;

in vec2 TextureCoordinates;

uniform mat3 normalMatrixTransform;

in vec3 Normal;
in vec3 FragmentPosition;
in mat3 TBN;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);

void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(diffuseMap, TextureCoordinates));
	const vec3 sampledSpecularValue = vec3(texture(specularMap, TextureCoordinates));

	vec3 normal = texture(normalMap, TextureCoordinates).rgb;
	normal = normal * 2.0 - 1.0;   
	normal = normalize(TBN * normal);

	//const vec3 normal = normalize(Normal);

	// camera always at origin because we are already in view space
	const vec3 cameraPosition = vec3(0.0f);
	const vec3 viewDirection = normalize(cameraPosition - FragmentPosition);
	vec3 result = vec3(0.0f);
	
	result += CalculateDirectionalLight(directionalLight, normal, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 invertedLightDirection = -normalize(inDirectionalLight.direction);

	const float diffuseValue = max(dot(inNormal, invertedLightDirection), 0.0f);
	
	float specularValue = 0.0f;
	if (dot(invertedLightDirection, inNormal) >= 0.0f)
	{
		const vec3 halfwayDir = normalize(invertedLightDirection + inViewDirection);
		specularValue = pow(max(dot(inNormal, halfwayDir), 0.0f), 32.0f);
	}

	const vec3 ambient = inDirectionalLight.ambient * inSampledDiffuseValue;
	const vec3 diffuse = inDirectionalLight.diffuse * diffuseValue * inSampledDiffuseValue * inDirectionalLight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * inDirectionalLight.intensity;

	return ambient + diffuse + specular;
}


