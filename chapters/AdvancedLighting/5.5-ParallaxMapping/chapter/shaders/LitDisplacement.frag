#version 460 core

out vec4 FragColor;

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	float intensity;
};


uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D displacementMap;

uniform sampler2D specularMap;

uniform DirectionalLight directionalLight;

in vec2 TextureCoordinates;

uniform mat3 normalMatrixTransform;

uniform float heightScale;

in vec3 Normal;
in vec3 FragmentPosition;
in mat3 TBN;

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue);

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
	// camera always at origin because we are already in view space
	const vec3 cameraPosition = vec3(0.0f);
	const vec3 viewDirection = TBN * normalize(cameraPosition - FragmentPosition);

	const vec2 parallaxTextureCoordinates = ParallaxMapping(TextureCoordinates, viewDirection);
	if(parallaxTextureCoordinates.x > 1.0 || parallaxTextureCoordinates.y > 1.0 || parallaxTextureCoordinates.x < 0.0 || parallaxTextureCoordinates.y < 0.0)
	{
    	discard;
	}

	const vec3 sampledDiffuseValue = vec3(texture(diffuseMap, parallaxTextureCoordinates));
	const vec3 sampledSpecularValue = vec3(0.0f);

	vec3 normal = texture(normalMap, parallaxTextureCoordinates).rgb;
	normal = normalize(normal * 2.0 - 1.0);   
	//normal = normalize(TBN * normal);

	vec3 result = vec3(0.0f);
	
	result += CalculateDirectionalLight(directionalLight, normal, viewDirection, sampledDiffuseValue, sampledSpecularValue);

	FragColor = vec4(result, 1.0f);
}

vec3 CalculateDirectionalLight(DirectionalLight inDirectionalLight, vec3 inNormal, vec3 inViewDirection, vec3 inSampledDiffuseValue, vec3 inSampledSpecularValue)
{
	const vec3 invertedLightDirection = -normalize(inDirectionalLight.direction);

	const float diffuseValue = max(dot(inNormal, invertedLightDirection), 0.0f);
	
	float specularValue = 0.0f;
	if (dot(invertedLightDirection, inNormal) >= 0.0f)
	{
		const vec3 halfwayDir = normalize(invertedLightDirection + inViewDirection);
		specularValue = pow(max(dot(inNormal, halfwayDir), 0.0f), 32.0f);
	}

	const vec3 ambient = inDirectionalLight.ambient * inSampledDiffuseValue;
	const vec3 diffuse = inDirectionalLight.diffuse * diffuseValue * inSampledDiffuseValue * inDirectionalLight.intensity;
	const vec3 specular = specularValue * inSampledSpecularValue * inDirectionalLight.intensity;

	return ambient + diffuse + specular;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));

    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;

    // depth of current layer
    float currentLayerDepth = 0.0;

    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * heightScale; 
    vec2 deltaTexCoords = P / numLayers;

	// get initial values
	vec2  currentTexCoords     = texCoords;
	float currentDepthMapValue = texture(displacementMap, currentTexCoords).r;
	
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;

		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(displacementMap, currentTexCoords).r;

		// get depth of next layer
		currentLayerDepth += layerDepth;  
	}

	// get texture coordinates before collision (reverse operations)
	vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(displacementMap, prevTexCoords).r - currentLayerDepth + layerDepth;
	
	// interpolation of texture coordinates
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

	return finalTexCoords; 
}


