#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out Color
{
	vec3 color;
} outColor;

void main()
{
	gl_Position = vec4(aPos.x,aPos.y, 0.0f, 1.0f);
	outColor.color = aColor;
}