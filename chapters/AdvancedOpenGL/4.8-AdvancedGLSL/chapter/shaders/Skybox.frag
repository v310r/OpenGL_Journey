#version 460 core

out vec4 FragColor;


uniform samplerCube skybox;

in vec3 TextureCoordinates;


void main()
{
	FragColor = texture(skybox, TextureCoordinates);
}
