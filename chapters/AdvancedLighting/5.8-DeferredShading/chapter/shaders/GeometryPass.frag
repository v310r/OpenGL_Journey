#version 460 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VertexOutput
{
	vec3 Normal;
	vec3 FragmentPosition;
	vec2 TextureCoordinates;
} fragmentInput;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shininess;
};

uniform Material material;

void main()
{
    gPosition = fragmentInput.FragmentPosition;

    gNormal = normalize(fragmentInput.Normal);

    gAlbedoSpec.rgb = texture(material.diffuse, fragmentInput.TextureCoordinates).rgb;
    gAlbedoSpec.a = texture(material.specular, fragmentInput.TextureCoordinates).r;
}
