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

in vec2 TextureCoordinates;

in vec3 Normal;
in vec3 FragmentPosition;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);


void main()
{
	const vec3 sampledDiffuseValue = vec3(texture(material.diffuse, TextureCoordinates));
	const vec3 sampledSpecularValue = vec3(texture(material.specular, TextureCoordinates));

	const vec3 normal = normalize(Normal);

	// camera always at origin because we are already in view space
	const vec3 cameraPosition = vec3(0.0f);
	const vec3 viewDirection = normalize(cameraPosition - FragmentPosition);
	
	vec3 result = CalculateDirectionalLight(directionalLight, normal, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	float near = 0.1; 
	float far  = 100.0;

	float ndc = gl_FragCoord.z * 2.0 - 1.0;
	float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));

	FragColor = vec4(vec3(linearDepth / far), 1.0);

	//FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 invertedLightDirection = -normalize(inDirectionalLight.direction);

	const float diffuseValue = max(dot(inNormal, invertedLightDirection), 0.0f);

	const vec3 reflectedDirection = reflect(inDirectionalLight.direction, inNormal);
	const float specularValue = pow(max(dot(inViewDirection, reflectedDirection), 0.0f), material.shininess);

	const vec3 ambient = inDirectionalLight.ambient * inSampledDiffuseValue;
	const vec3 diffuse = inDirectionalLight.diffuse * diffuseValue * inSampledDiffuseValue * inDirectionalLight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * inDirectionalLight.intensity;

	return ambient + diffuse + specular;
}
