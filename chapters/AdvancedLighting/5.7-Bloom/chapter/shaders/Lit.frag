#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

#define POINT_LIGHTS_NUM 5

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
	vec3 Normal;
	vec3 FragmentPosition;
	vec3 WorldFragmentPosition;
	vec2 TextureCoordinates;
} fragmentInput;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);
vec3 CalculatePointLight(PointLight inPointLight, vec3 inNormal, vec3 inFragmentPosition, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);

float ShadowCalculation(vec3 inFragmentPosition);

uniform Material material;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[POINT_LIGHTS_NUM];

uniform vec3 worldLightPos;

uniform samplerCube depthMap;

uniform float shadowFarPlane;

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
	
	//result += CalculateDirectionalLight(directionalLight, normal, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	for (int i = 0; i < POINT_LIGHTS_NUM; i++)
	{
		result += CalculatePointLight(pointLights[i], normal, fragmentInput.FragmentPosition, viewDirection, sampledDiffuseValue, sampledSpecularValue);
	}

	const vec3 color = sampledDiffuseValue.rgb;
	const vec3 ambient = 0.3 * color;

	result += ambient;

	FragColor = vec4(result, 1.0f);

	const float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	if (brightness > 1.0f)
	{
		BrightColor = vec4(FragColor.rgb, 1.0f);
	}
	else
	{
		BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
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

	//const float shadow = ShadowCalculation(fragmentInput.LightViewFragmentPosition, inNormal, invertedLightDirection);
	//const vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular));

	return ambient + diffuse + specular;
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

float ShadowCalculation(vec3 inFragmentPosition)
{
	vec3 lightToFragment = inFragmentPosition - worldLightPos;

	const float currentDepth = length(lightToFragment);

	vec3 sampleOffsetDirections[20] = vec3[]
	(
	vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
	vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
	vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
	vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
	vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
	); 

	float shadow = 0.0;
	float bias   = 0.15;
	int samples  = 20;
	float diskRadius = 0.05;
	for(int i = 0; i < samples; ++i)
	{
	    float closestDepth = texture(depthMap, lightToFragment + sampleOffsetDirections[i] * diskRadius).r;
		if (closestDepth > 1.0f)
		{
			continue;
		}
		
	    closestDepth *= shadowFarPlane;   // undo mapping [0;1]
	    if(currentDepth - bias > closestDepth)
		{
	        shadow += 1.0;
		}
	}

	shadow /= float(samples);

	if (shadow > 1.0f)
	{
		shadow = 0.0f;
	}

	return shadow;
}