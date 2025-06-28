#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D screenTexture;

void main()
{
	// inverse
	FragColor = vec4(vec3(1.0f - texture(screenTexture, TextureCoordinates)), 1.0f);
}
