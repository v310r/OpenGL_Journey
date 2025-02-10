#version 460 core

out vec4 FragColor;

in vec3 LightingResult;

void main()
{
	FragColor = vec4(LightingResult, 1.0f);
}
