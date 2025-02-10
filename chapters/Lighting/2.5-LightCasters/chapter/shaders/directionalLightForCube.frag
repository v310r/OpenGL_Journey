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

struct Light
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	float intensity;
};

uniform Light light;

in vec2 TextureCoordinates;

in vec3 Normal;
in vec3 FragmentPosition;

void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(material.diffuse, TextureCoordinates));
	const vec3 ambient = sampledDiffuseValue * light.ambient;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(-light.direction);
	const float diffuseValue = max(dot(normal, lightDir), 0.0f);
	const vec3 diffuse = (diffuseValue * sampledDiffuseValue) * light.diffuse * light.intensity;

	// const vec3 cameraPosition = vec3(0.0f);
	// const vec3 viewDir = normalize(cameraPosition - FragmentPosition);
	// camera always at origin because we are already in view space
	const vec3 viewDir = normalize(-FragmentPosition);
	const vec3 reflectDir = reflect(-lightDir, normal);
	const float cameraBasedSpecularValue = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	const vec3 sampledSpecularValue = vec3(texture(material.specular, TextureCoordinates));
	const vec3 specular = sampledSpecularValue * cameraBasedSpecularValue * light.intensity;

	vec3 emission = vec3(0.0f);
	if (sampledSpecularValue.r == 0.0f)
	{
		emission = vec3(texture(material.emission, TextureCoordinates));
	}

	const vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0f);
}
