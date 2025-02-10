#version 460 core

out vec4 FragColor;

// cube color
uniform vec3 color;

uniform vec3 lightColor;

void main()
{
	FragColor = vec4(color * lightColor, 1.0f);
}
