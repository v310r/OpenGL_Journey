#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in Color
{
	vec3 color;
} inColor[];

out vec3 outColor;

void BuildHouse(vec4 position)
{    
    gl_Position = position + vec4(-0.1, -0.1, 0.0, 0.0);    // 1:bottom-left
    EmitVertex();   
    gl_Position = position + vec4( 0.1, -0.1, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.1,  0.1, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.1,  0.1, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.2, 0.0, 0.0);    // 5:top
	outColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
	outColor = inColor[0].color;
	BuildHouse(gl_in[0].gl_Position);
}