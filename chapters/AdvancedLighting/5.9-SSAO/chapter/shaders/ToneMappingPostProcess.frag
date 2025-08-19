#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D screenTexture;

uniform float exposure;

void main()
{
    const vec3 hdrColor = texture(screenTexture, TextureCoordinates).rgb;

    const float gamma = 2.2f;

    //vec3 mapped = hdrColor / (hdrColor + vec3(1.0f));

    vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);

    mapped = pow(mapped, vec3(1.0f / gamma));

	FragColor = vec4(mapped, 1.0f);
}
