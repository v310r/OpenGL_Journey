#version 460 core

out vec4 FragColor;

// light source color
uniform vec3 color;

void main()
{
	FragColor = vec4(color, 1.0f);
}
