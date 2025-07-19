#version 460 core

in vec4 FragPos;

uniform float farPlane;

void main()
{
	float lightDistance = length(FragPos.xyz);

    lightDistance = lightDistance / farPlane;

    gl_FragDepth = lightDistance;
}
