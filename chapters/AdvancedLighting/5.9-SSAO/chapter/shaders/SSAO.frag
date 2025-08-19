#version 460 core

out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noiseTexture;


in vec2 TextureCoordinates;

const float kernelSize = 64;
const float radius = 0.5f;

uniform vec3 samples[64];
uniform mat4 projection;

// tile noise texture over screen, based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); // screen = 800x600

const float bias = 0.025;

void main()
{
	vec3 fragPos   = texture(gPosition, TextureCoordinates).xyz;
	vec3 normal    = texture(gNormal, TextureCoordinates).rgb;
	vec3 randomVec = texture(noiseTexture, TextureCoordinates * noiseScale).xyz;

	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN       = mat3(tangent, bitangent, normal);


	float occlusion = 0.0;
	for(int i = 0; i < kernelSize; ++i)
	{
		// get sample position
		vec3 samplePos = TBN * samples[i]; // from tangent to view-space
		samplePos = fragPos + samplePos * radius;
		
		vec4 offset = vec4(samplePos, 1.0);
		offset      = projection * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz  = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0 

		float sampleDepth = texture(gPosition, offset.xy).z;

		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
		occlusion       += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = vec4(vec3(occlusion), 1.0f);
}

