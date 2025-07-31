#version 460 core

out vec4 FragColor;

in vec2 TextureCoordinates;

uniform sampler2D scene;
uniform sampler2D bloomBlur;

uniform float exposure;

void main()
{
    vec3 hdrColor = texture(scene, TextureCoordinates).rgb;      
    vec3 bloomColor = texture(bloomBlur, TextureCoordinates).rgb;
    hdrColor += bloomColor; // additive blending

    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    // also gamma correct while we're at it
    const float gamma = 2.2;
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}
