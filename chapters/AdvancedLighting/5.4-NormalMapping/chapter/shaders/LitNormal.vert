#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 model;

layout (std140, binding = 0) uniform ViewProj
{
    mat4 view;
    mat4 projection;
};

uniform mat3 normalMatrixTransform;

out vec3 Normal;
out vec3 FragmentPosition;
out vec2 TextureCoordinates;
out mat3 TBN;

void main()
{
	vec3 T = normalize(vec3(view * model * vec4(aTangent,   0.0)));
   	vec3 B = normalize(vec3(view * model * vec4(aBitangent, 0.0)));
   	vec3 N = normalize(vec3(view * model * vec4(aNormal,    0.0)));
   	TBN = mat3(T, B, N);

	gl_Position = projection * view  * model * vec4(aPos, 1.0f);
	Normal = normalMatrixTransform * aNormal;
	FragmentPosition = vec3(view * model * vec4(aPos, 1.0f));
	TextureCoordinates = aTexCoords;
}