#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D image;


void main()
{
	FragColor = vec4(vec3(texture(image, TextureCoordinates).r), 1.0f);
}
