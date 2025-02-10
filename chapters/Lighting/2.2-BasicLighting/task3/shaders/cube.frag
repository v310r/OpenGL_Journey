#version 460 core

out vec4 FragColor;

// cube color
uniform vec3 color;

uniform vec3 lightColor;
uniform vec3 lightPosition;

in vec3 Normal;
in vec3 FragmentPosition;

void main()
{
	const float ambientStrength = 0.1;
	const vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragmentPosition);
	const float diff = max(dot(normal, lightDir), 0.0f);
	const vec3 diffuse = diff * lightColor;

	const float specularStrength = 0.5f;
	// const vec3 cameraPosition = vec3(0.0f);
	// const vec3 viewDir = normalize(cameraPosition - FragmentPosition);
	// camera always at origin because we are already in view space
	const vec3 viewDir = normalize(-FragmentPosition);
	const vec3 reflectDir = reflect(-lightDir, normal);
	const float specularValue = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	const vec3 specular = specularStrength * specularValue * lightColor;

	vec3 result = (ambient + diffuse + specular) * color;
	FragColor = vec4(result, 1.0f);
}
