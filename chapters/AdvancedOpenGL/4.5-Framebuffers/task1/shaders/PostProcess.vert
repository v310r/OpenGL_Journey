#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 TextureCoordinates;

void main()
{
	const float xPos = (aPos.x * 0.5) + 0.5;
	const float yPos = (aPos.y * 0.5) + 0.5;

	gl_Position = vec4(xPos, yPos, 0.0, 1.0);
	TextureCoordinates = aTexCoords;
}