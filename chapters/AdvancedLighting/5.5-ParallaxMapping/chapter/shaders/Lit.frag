#version 460 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	sampler2D emission;

	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	float intensity;
};


uniform Material material;

uniform DirectionalLight directionalLight;

in vec2 TextureCoordinates;

uniform mat3 normalMatrixTransform;

in vec3 Normal;
in vec3 FragmentPosition;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);

void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(material.diffuse, TextureCoordinates));
	const vec3 sampledSpecularValue = vec3(texture(material.specular, TextureCoordinates));

	const vec3 normal = normalize(Normal);

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
		specularValue = pow(max(dot(inNormal, halfwayDir), 0.0f), material.shininess);
	}

	const vec3 ambient = inDirectionalLight.ambient * inSampledDiffuseValue;
	const vec3 diffuse = inDirectionalLight.diffuse * diffuseValue * inSampledDiffuseValue * inDirectionalLight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * inDirectionalLight.intensity;

	return ambient + diffuse + specular;
}


