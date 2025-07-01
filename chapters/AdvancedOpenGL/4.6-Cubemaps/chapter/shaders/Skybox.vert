#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out vec3 TextureCoordinates;

void main()
{
	TextureCoordinates = aPos;
	vec4 pos = projection * view * vec4(aPos, 1.0);
	
	// optimization! we are setting z to w, so that depth value would be equal to 1.0 when testing depth, and OpenGL would use GL_LEQUAL
    gl_Position = pos.xyww;
}