#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;

layout (std140, binding = 0) uniform ViewProj
{
    mat4 view;
    mat4 projection;
};

uniform mat3 normalMatrixTransform;

out VertexOutput
{
	vec3 Normal;
	vec3 FragmentPosition;
	vec2 TextureCoordinates;
} vertexOutput;

void main()
{
	gl_Position = projection * view  * model * vec4(aPos, 1.0f);

	vertexOutput.Normal = normalMatrixTransform * aNormal;
	vertexOutput.TextureCoordinates = aTexCoords;

	const vec3 worldFragmentPosition = vec3(model * vec4(aPos, 1.0f));
	//vertexOutput.FragmentPosition = vec3(view * vec4(worldFragmentPosition, 1.0f));
	vertexOutput.FragmentPosition = vec3(view * vec4(worldFragmentPosition, 1.0f));
}