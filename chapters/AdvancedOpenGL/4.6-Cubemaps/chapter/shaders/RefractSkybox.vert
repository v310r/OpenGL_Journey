#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat3 normalMatrixTransform;

out vec3 Normal;
out vec3 Position;

void main()
{
	Normal = normalMatrixTransform * aNormal;
	Position = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * vec4(Position, 1.0f);
}