#version 460 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragmentPosition;

struct Material
{
	vec3 diffuse;
};

uniform Material material;

void main()
{
	FragColor = vec4(material.diffuse, 1.0f);
}
