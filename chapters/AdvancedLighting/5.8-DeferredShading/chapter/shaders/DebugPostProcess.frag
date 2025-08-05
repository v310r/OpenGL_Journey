#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D image;


void main()
{
	FragColor = texture(image, TextureCoordinates);
}
