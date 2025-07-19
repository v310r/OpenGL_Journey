#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D screenTexture;

void main()
{
    //const float near = 0.1f;
    //const float far = 300.0f;
//
    //const float ndc = texture(screenTexture, TextureCoordinates).r * 2.0 - 1.0;
	//const float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));
//
	//FragColor = vec4(vec3(linearDepth / far), 1.0);

    const float depthValue = texture(screenTexture, TextureCoordinates).r;
	FragColor = vec4(vec3(depthValue), 1.0);
}
