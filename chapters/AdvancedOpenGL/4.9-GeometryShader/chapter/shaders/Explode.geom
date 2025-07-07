#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TextureCoordinates[];

out vec2 FragmentedTextureCoordinates;

uniform float time;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

vec4 Explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f);
    vec3 distance = direction * magnitude;
    return position + vec4(distance, 1.0f);
}

void main()
{
	vec3 normal = GetNormal();

    for (int i = 0; i < 3; i++)
    {
        vec4 explodedDistanceOffset = Explode(gl_in[i].gl_Position, normal);
        gl_Position = explodedDistanceOffset;
        FragmentedTextureCoordinates = TextureCoordinates[i];
        EmitVertex();
    }


    EndPrimitive();
}