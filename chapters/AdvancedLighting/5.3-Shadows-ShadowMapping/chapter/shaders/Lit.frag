#version 460 core

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

in VertexOutput
{
	vec4 LightViewFragmentPosition;
	vec3 Normal;
	vec3 FragmentPosition;
	vec2 TextureCoordinates;
} fragmentInput;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);
vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);

float ShadowCalculation(vec4 inLightViewFragmentPosition,  vec3 inNormal, vec3 inInvertedLightDirection);

uniform Material material;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;

uniform sampler2D shadowMap;

void main()
{
	const vec3 sampledDiffuseValueSRGB = vec3(texture(material.diffuse, fragmentInput.TextureCoordinates));

	const vec3 sampledDiffuseValue = pow(sampledDiffuseValueSRGB, vec3(2.2));

	const vec3 sampledSpecularValue = vec3(texture(material.specular, fragmentInput.TextureCoordinates));

	const vec3 normal = normalize(fragmentInput.Normal);

	// camera always at origin because we are already in view space
	const vec3 cameraPosition = vec3(0.0f);
	const vec3 viewDirection = normalize(cameraPosition - fragmentInput.FragmentPosition);
	vec3 result = vec3(0.0f);
	
	result += CalculateDirectionalLight(directionalLight, normal, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	//result += CalculatePointLight(pointLight, normal, fragmentInput.FragmentPosition, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 invertedLightDirection = -normalize(inDirectionalLight.direction);

	const float diffuseValue = max(dot(invertedLightDirection, inNormal), 0.0f);

	//const vec3 reflectedDirection = reflect(inDirectionalLight.direction, inNormal);
	//const float specularValue = pow(max(dot(inViewDirection, reflectedDirection), 0.0f), material.shininess);

	float specularValue = 0.0f;
	if (dot(invertedLightDirection, inNormal) >= 0.0f)
	{
		const vec3 halfwayDir = normalize(invertedLightDirection + inViewDirection);
		specularValue = pow(max(dot(inNormal, halfwayDir), 0.0f), material.shininess);
	}

	const vec3 ambient = inDirectionalLight.ambient * inSampledDiffuseValue;
	const vec3 diffuse = inDirectionalLight.diffuse * diffuseValue * inSampledDiffuseValue * inDirectionalLight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * inDirectionalLight.intensity;

	const vec3 color = inSampledDiffuseValue.rgb;

	const float shadow = ShadowCalculation(fragmentInput.LightViewFragmentPosition, inNormal, invertedLightDirection);
	const vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular));

	return lighting;
}

vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 lightDirection = normalize(inFragmentPosition - inPointLight.position);

	const float diffuseValue = max(dot(inNormal, -lightDirection), 0.0f);

	float specularValue = 0.0f;
	if (dot(-lightDirection, inNormal) >= 0.0f)
	{
		const vec3 halfwayDir = normalize(-lightDirection + inViewDirection);
		specularValue = pow(max(dot(inNormal, halfwayDir), 0.0f), material.shininess);
	}

	const float distance = length(inPointLight.position - inFragmentPosition);
	const float attenuation = 1.0f / (inPointLight.constant + inPointLight.linear * distance + inPointLight.quadratic * (distance * distance));

	vec3 diffuse = inPointLight.diffuse * diffuseValue * inSampledDiffuseValue * inPointLight.intensity;
	vec3 specular = specularValue * inSampledSpecularValue * inPointLight.intensity;

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

float ShadowCalculation(vec4 inLightViewFragmentPosition,  vec3 inNormal, vec3 inInvertedLightDirection)
{
	// perspective divide
	vec3 projectedCoords = inLightViewFragmentPosition.xyz / inLightViewFragmentPosition.w;

	//[0, 1]
	projectedCoords = projectedCoords * 0.5f + 0.5f;

	const float closestDepth = texture(shadowMap, projectedCoords.xy).r;
	const float currentDepth = projectedCoords.z;

	float bias = max(0.05 * (1.0 - dot(inNormal, inInvertedLightDirection)), 0.005); 

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projectedCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}

	shadow /= 9.0f;

	//if (currentDepth > 1.0f)
	//{
	//	shadow = 0.0f;
	//}

	return shadow;
}