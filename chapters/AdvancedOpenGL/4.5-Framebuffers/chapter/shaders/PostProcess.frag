#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D screenTexture;

void main()
{
	// inverse
	//FragColor = vec4(vec3(1.0f - texture(screenTexture, TextureCoordinates)), 1.0f);

	// grayscale
	//FragColor = texture(screenTexture, TextureCoordinates);
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //FragColor = vec4(average, average, average, 1.0);

	const float offsetToFragmentNeighbour = 1.0f / 300.0f;

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

    //float sharpenKernel[9] = float[](
    //    -1, -1, -1,
    //    -1,  9, -1,
    //    -1, -1, -1
    //);
    
	//float blurKernel[9] = float[]
	//(
    //	1.0 / 16, 2.0 / 16, 1.0 / 16,
    //	2.0 / 16, 4.0 / 16, 2.0 / 16,
    //	1.0 / 16, 2.0 / 16, 1.0 / 16  
	//);

	float edgeDetectionKernel[9] = float[]
	(
       1, 1, 1,
       1, -8, 1,
       1, 1, 1
    );

    vec3 sampleTextureCoords[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTextureCoords[i] = vec3(texture(screenTexture, TextureCoordinates.st + fragmentOffsets[i]));
    }
	
    vec3 finalFragmentColor = vec3(0.0);
    for(int i = 0; i < 9; i++)
	{
        finalFragmentColor += sampleTextureCoords[i] * edgeDetectionKernel[i];
	}

	FragColor = vec4(finalFragmentColor, 1.0f);

}
