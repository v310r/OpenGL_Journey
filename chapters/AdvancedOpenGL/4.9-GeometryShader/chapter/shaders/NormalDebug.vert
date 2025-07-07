#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;

out vec3 normal;

layout (std140, binding = 0) uniform ViewProj
{
    mat4 view;
    mat4 projection;
};

uniform mat3 normalMatrixTransform;

void main()
{
	gl_Position =  view  * model * vec4(aPos, 1.0f);
    normal = normalMatrixTransform * aNormal;
}