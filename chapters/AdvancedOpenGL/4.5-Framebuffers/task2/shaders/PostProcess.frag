#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D screenTexture;

void main()
{
	const float offsetToFragmentNeighbour = 1.0f / 500.0f;

	vec2 fragmentOffsets[9] = vec2[]
	(
        vec2(-offsetToFragmentNeighbour,  offsetToFragmentNeighbour), 							// top-left
        vec2( 0.0f,    offsetToFragmentNeighbour), 						// top-center
        vec2( offsetToFragmentNeighbour,  offsetToFragmentNeighbour), 	// top-right
        vec2(-offsetToFragmentNeighbour,  0.0f),   						// center-left

        vec2( 0.0f,    0.0f),   										// center-center

        vec2( offsetToFragmentNeighbour,  0.0f),   						// center-right
        vec2(-offsetToFragmentNeighbour, -offsetToFragmentNeighbour), 	// bottom-left
        vec2( 0.0f,   -offsetToFragmentNeighbour), 						// bottom-center
        vec2( offsetToFragmentNeighbour, -offsetToFragmentNeighbour)  	// bottom-right    
    );

	float myCustomKernel[9] = float[]
	(
       9, 2, -3,
       4, -30, 6,
       -3, 8, 9
    );

    vec3 sampleTextureCoords[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTextureCoords[i] = vec3(texture(screenTexture, TextureCoordinates.st + fragmentOffsets[i]));
    }
	
    vec3 finalFragmentColor = vec3(0.0);
    for(int i = 0; i < 9; i++)
	{
        finalFragmentColor += sampleTextureCoords[i] * myCustomKernel[i];
	}

	FragColor = vec4(finalFragmentColor, 1.0f);

}
