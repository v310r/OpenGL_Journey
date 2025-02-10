#version 460 core

out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
	vec3 lightDir = normalize(light.position - FragmentPosition);
	const float diffuseValue = max(dot(normal, lightDir), 0.0f);
	const vec3 diffuse = (diffuseValue * sampledDiffuseValue) * light.diffuse;

	// const vec3 cameraPosition = vec3(0.0f);
	// const vec3 viewDir = normalize(cameraPosition - FragmentPosition);
	// camera always at origin because we are already in view space
	const vec3 viewDir = normalize(-FragmentPosition);
	const vec3 reflectDir = reflect(-lightDir, normal);
	const float cameraBasedSpecularValue = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	const vec3 specular = vec3(texture(material.specular, TextureCoordinates)) * cameraBasedSpecularValue * light.specular;

	const vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}
