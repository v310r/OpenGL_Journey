#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrixTransform;

// cube color
uniform vec3 color;

uniform vec3 lightColor;
uniform vec3 lightPosition;

uniform vec3 cameraPosition;

out vec3 LightingResult;

void main()
{
	gl_Position = projection * view  * model * vec4(aPos, 1.0f);
	vec3 Normal = normalMatrixTransform * aNormal;
	vec3 FragmentPosition = vec3(model * vec4(aPos, 1.0f));

	const float ambientStrength = 0.1;
	const vec3 ambient = ambientStrength * lightColor;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragmentPosition);
	const float diff = max(dot(normal, lightDir), 0.0f);
	const vec3 diffuse = diff * lightColor;

	const float specularStrength = 0.5f;
	const vec3 viewDir = normalize(cameraPosition - FragmentPosition);
	const vec3 reflectDir = reflect(-lightDir, normal);
	const float specularValue = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);
	const vec3 specular = specularStrength * specularValue * lightColor;

	LightingResult = (ambient + diffuse + specular) * color;
}