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

uniform Material material;

uniform DirectionalLight directionalLight;

in vec2 FragmentedTextureCoordinates;


vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);


void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(material.diffuse, FragmentedTextureCoordinates));

	FragColor = vec4(sampledDiffuseValue, 1.0f);
}
