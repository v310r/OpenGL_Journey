#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;
  
uniform sampler2D imageSSAO;

void main()
{
	vec2 texelSize = 1.0 / vec2(textureSize(imageSSAO, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(imageSSAO, TextureCoordinates + offset).r;
        }
    }

    FragColor = vec4(vec3(result / (4.0 * 4.0)), 1.0f);
}

