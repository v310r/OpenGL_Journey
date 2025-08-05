#version 460 core

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

#define POINT_LIGHTS_NUM 5

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

in vec2 TextureCoordinates;

vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, float inSampledSpecularValue);

uniform PointLight pointLights[POINT_LIGHTS_NUM];

void main()
{
	const vec4 sampledAlbedo = texture(gAlbedoSpec, TextureCoordinates);
	const vec3 sampledDiffuseValue = sampledAlbedo.rgb;

	const float sampledSpecularValue = sampledAlbedo.a;

	const vec3 normal = normalize(texture(gNormal, TextureCoordinates).rgb);

	const vec3 FragmentPosition = texture(gPosition, TextureCoordinates).rgb;

	// camera always at origin because we are already in view space
	const vec3 cameraPosition = vec3(0.0f);
	const vec3 viewDirection = normalize(cameraPosition - FragmentPosition);
	vec3 result = vec3(0.0f);

	for (int i = 0; i < POINT_LIGHTS_NUM; i++)
	{
		result += CalculatePointLight(pointLights[i], normal, FragmentPosition, viewDirection, sampledDiffuseValue, sampledSpecularValue);
	}

	const vec3 color = sampledDiffuseValue.rgb;
	const vec3 ambient = 0.3 * color;

	result += ambient;

	FragColor = vec4(result, 1.0f);
}

vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, float inSampledSpecularValue)
{
	const vec3 lightDirection = normalize(inFragmentPosition - inPointLight.position);

	const float diffuseValue = max(dot(inNormal, -lightDirection), 0.0f);

	float specularValue = 0.0f;
	if (dot(-lightDirection, inNormal) >= 0.0f)
	{
		const vec3 halfwayDir = normalize(-lightDirection + inViewDirection);
		specularValue = pow(max(dot(inNormal, halfwayDir), 0.0f), 32.0f);
	}

	const float distance = length(inPointLight.position - inFragmentPosition);
	const float attenuation = 1.0f / (inPointLight.constant + inPointLight.linear * distance + inPointLight.quadratic * (distance * distance));

	vec3 diffuse = inPointLight.diffuse * diffuseValue * inSampledDiffuseValue * inPointLight.intensity;
	vec3 specular = vec3(specularValue * inSampledSpecularValue * inPointLight.intensity);

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}
