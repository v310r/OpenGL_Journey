#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;
in vec3 Normal;
in vec3 FragmentPosition;


void main()
{
	if (gl_FragCoord.x < 400)
	{
		FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}
