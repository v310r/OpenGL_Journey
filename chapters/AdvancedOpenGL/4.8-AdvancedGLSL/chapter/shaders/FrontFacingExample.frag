#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
	if (gl_FrontFacing)
	{
		FragColor = texture(frontTexture, TextureCoordinates);
	}
	else
	{
		FragColor = texture(backTexture, TextureCoordinates);
	}
}
