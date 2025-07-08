#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 model;

layout (std140, binding = 0) uniform ViewProj
{
    mat4 view;
    mat4 projection;
};

uniform mat3 normalMatrixTransform;

uniform mat3 instanceRotation;

out vec3 Normal;
out vec3 FragmentPosition;
out vec2 TextureCoordinates;

void main()
{
	mat4 modelView = view * instanceMatrix;
    mat3 normalMatrix = transpose(inverse(mat3(modelView)));

	gl_Position = projection * modelView * vec4(aPos, 1.0f);
	Normal = normalMatrix * aNormal;
	FragmentPosition = vec3(view * model * vec4(aPos, 1.0f));
	TextureCoordinates = aTexCoords;
}