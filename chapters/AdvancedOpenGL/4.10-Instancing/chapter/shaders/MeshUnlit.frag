#version 460 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse1;
	sampler2D specular1;
	sampler2D emission1;

	float shininess;
};

uniform Material material;

in vec2 TextureCoordinates;

in vec3 Normal;
in vec3 FragmentPosition;


void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(material.diffuse1, TextureCoordinates));

	FragColor = vec4(sampledDiffuseValue, 1.0f);
}
