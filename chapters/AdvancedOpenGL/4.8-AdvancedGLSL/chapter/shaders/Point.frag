#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;


void main()
{

	FragColor = vec4(vec3(0.0f, 0.5f, 0.5f), 1.0f);
}
