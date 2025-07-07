#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;


layout (std140, binding = 0) uniform ViewProj
{
    mat4 view;
    mat4 projection;
};


in vec3 normal[];

uniform float time;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();

    gl_Position = projection * (gl_in[index].gl_Position + vec4(normal[index], 0.0f) * 0.4f);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    GenerateLine(0);
    GenerateLine(1);
    GenerateLine(2);
}