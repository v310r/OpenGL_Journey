#version 460 core

#define POINT_LIGHTS_NUM 4  


out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
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

struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	float intensity;

	float constant;
	float linear;
	float quadratic;
};

struct Spotlight
{
	vec3 position;
	vec3 direction;

	vec3 diffuse;

	float intensity;

	// dot product of inner angle
	float innerDot;

	// dot product of outer angle
	float outerDot;
};

uniform Material material;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[POINT_LIGHTS_NUM];
uniform Spotlight spotlight;

in vec2 TextureCoordinates;

in vec3 Normal;
in vec3 FragmentPosition;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);
vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);
vec3 CalculateSpotlight(Spotlight inSpotlight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);

void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(material.diffuse, TextureCoordinates));
	const vec3 sampledSpecularValue = vec3(texture(material.specular, TextureCoordinates));

	const vec3 normal = normalize(Normal);

	// camera always at origin because we are already in view space
	const vec3 cameraPosition = vec3(0.0f);
	const vec3 viewDirection = normalize(cameraPosition - FragmentPosition);
	
	vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	for (int i = 0; i < POINT_LIGHTS_NUM; i++)
	{
		result += CalculatePointLight(pointLights[i], normal, FragmentPosition, viewDirection, sampledDiffuseValue, sampledSpecularValue);
	}

	result += CalculateSpotlight(spotlight, normal, FragmentPosition, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	// masking emission (it will override light calculations where emission RGB > 0.0f)
	const vec3 emission = vec3(texture(material.emission, TextureCoordinates));
	const float maxEmission = max(max(emission.r, emission.g), emission.b);
	const float mask = step(0.001f, maxEmission);
	result = mix(result, emission, mask);

	FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 invertedLightDirection = -normalize(inDirectionalLight.direction);

	const float diffuseValue = max(dot(inNormal, invertedLightDirection), 0.0f);

	const vec3 reflectedDirection = reflect(inDirectionalLight.direction, inNormal);
	const float specularValue = pow(max(dot(inViewDirection, reflectedDirection), 0.0f), material.shininess);

	const vec3 ambient = inDirectionalLight.ambient * inSampledDiffuseValue;
	const vec3 diffuse = inDirectionalLight.diffuse * diffuseValue * inSampledDiffuseValue * inDirectionalLight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * inDirectionalLight.intensity;

	return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 lightDirection = normalize(inFragmentPosition - inPointLight.position);

	const float diffuseValue = max(dot(inNormal, -lightDirection), 0.0f);

	const vec3 reflectedDirection = reflect(lightDirection, inNormal);
	const float specularValue = pow(max(dot(inViewDirection, reflectedDirection), 0.0f), material.shininess);

	const float distance = length(inPointLight.position - inFragmentPosition);
	const float attenuation = 1.0f / (inPointLight.constant + inPointLight.linear * distance + inPointLight.quadratic * (distance * distance));

	vec3 diffuse = inPointLight.diffuse * diffuseValue * inSampledDiffuseValue;
	vec3 specular = specularValue * inSampledSpecularValue * inPointLight.intensity;

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

vec3 CalculateSpotlight(Spotlight inSpotlight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 lightDirection = normalize(inFragmentPosition - inSpotlight.position);

	const float diffuseValue = max(dot(inNormal, -lightDirection), 0.0f);

	const vec3 reflectedDirection = reflect(lightDirection, inNormal);
	const float specularValue = pow(max(dot(inViewDirection, reflectedDirection), 0.0f), material.shininess);

	const float theta = dot(lightDirection, normalize(inSpotlight.direction));
	const float epsilon = inSpotlight.innerDot - inSpotlight.outerDot;
	const float interpolatedLightIntensityBetweenCones = clamp((theta - inSpotlight.outerDot) / epsilon, 0.0f, 1.0f);

	const vec3 diffuse = inSpotlight.diffuse * diffuseValue * inSampledDiffuseValue * interpolatedLightIntensityBetweenCones * inSpotlight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * interpolatedLightIntensityBetweenCones * inSpotlight.intensity;

	return diffuse + specular;
}
