#version 460 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shininess;
};

uniform Material material;

in vec2 TextureCoordinates;

in vec3 Normal;
in vec3 FragmentPosition;

void main()
{
	const vec4 sampledDiffuseValue = vec4(texture(material.diffuse, TextureCoordinates));

	if (sampledDiffuseValue.a < 0.1f)
	{
		discard;
	}

	FragColor = sampledDiffuseValue;
}
