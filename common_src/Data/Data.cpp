#include "Data.h"


// TODO: Make compact version for cube; use 24 vertices instead of 36

float g_CubeVerticesWithNormals[] =
{
	// positions			// normal coords

	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,		// Back face
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,		// Front face
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,

	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,		// Left face
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,

	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,		// Right face
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,		// Bottom face
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,		// Top face
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f
};

float g_TexturedCubeVertices[] =
{
	// positions         // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  // back side
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  // front side
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // left side
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  // right side
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  // down side
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  // up side
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


float g_TexturedCubeVerticesWithNormals[] =
{
	// positions			// normal coords	// texture coords

	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,		// Back face
	0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, -1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, -1.0f,	0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
	0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// Front face
	0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		// Left face
	-0.5f, -0.5f, -0.5f,	-1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		-1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		// Right face
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 1.0f,
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,		// Bottom face
	0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,		0.0f, -1.0f, 0.0f,	0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, -1.0f, 0.0f,	0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
	0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// Top face
	0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f
};

//
// Can't map texture indices (also normals, etc.) correctly in such a way, for up, bottom, left and right sides we will have incorrect texture sampling
// 
// Instead of 36 vertices, you need 24 vertices (4 vertices for each face)
//
//float g_CompactTexturedCubeVertices[] =
//{
//	// positions			// texture coords
//	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,	// Bottom-left (Back)
//	0.5f, -0.5f, -0.5f,		1.0, 0.0f,	// Bottom-right (Back)
//	0.5f,  0.5f, -0.5f,		1.0f, 1.0f,	// Top-right (Back)
//	-0.5, 0.5f, -0.5f,		0.0f, 1.0f,	// Top-left (Back)
//
//	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,	// Bottom-left (Front)
//	0.5f, -0.5f, 0.5f,		1.0, 0.0f,	// Bottom-right (Front)
//	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,	// Top-right (Front)
//	-0.5, 0.5f, 0.5f,		0.0f, 1.0f,	// Top-left (Front)
//};

float g_CompactCubeVertices[] =
{
	// positions
	-0.5f, -0.5f, -0.5f,	// Bottom-left (Back)
	0.5f, -0.5f, -0.5f,		// Bottom-right (Back)
	0.5f,  0.5f, -0.5f,		// Top-right (Back)
	-0.5, 0.5f, -0.5f,		// Top-left (Back)

	-0.5f, -0.5f, 0.5f,		// Bottom-left (Front)
	0.5f, -0.5f, 0.5f,		// Bottom-right (Front)
	0.5f, 0.5f, 0.5f,		// Top-right (Front)
	-0.5, 0.5f, 0.5f		// Top-left (Front)
};

unsigned int g_CubeIndices[] =
{
	// Back face
	0, 1, 2,
	2, 3, 0,

	// Front face
	4, 5, 6,
	6, 7, 4,

	// Left face
	0, 4, 7,
	7, 3, 0,

	// Right face
	1, 5, 6,
	6, 2, 1,

	// Bottom face
	0, 1, 5,
	5, 4, 0,

	// Top face
	3, 2, 6,
	6, 7, 3
};

size_t g_CubeVerticesWithNormalsSizeInBytes = sizeof(g_CubeVerticesWithNormals);
size_t g_TexturedCubeVerticesSizeInBytes = sizeof(g_TexturedCubeVertices);
size_t g_TexturedCubeVerticesWithNormalsSizeInBytes = sizeof(g_TexturedCubeVerticesWithNormals);


//size_t g_CompactTexturedCubeVerticesSizeInBytes = sizeof(g_CompactTexturedCubeVertices);
size_t g_CompactCubeVerticesSizeInBytes = sizeof(g_CompactCubeVertices);
size_t g_CubeIndicesSizeInBytes = sizeof(g_CubeIndices);


extern float g_QuadVertices[] =
{
	// Positions
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};

float g_QuadVerticesWithNormals[] =
{
	// Positions			// Normals
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,  // Bottom Left
	 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,   // Bottom Right
	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,  // Top Right
	-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f  // Top Left
};

extern float g_TexturedQuadVertices[] = 
{
	// Positions			// TexCoords
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,  // Bottom Left
	 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,   // Bottom Right
	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,  // Top Right
	-0.5f,  0.5f, 0.0f,		0.0f, 1.0f  // Top Left
};

extern float g_TexturedQuadVerticesWithNormals[] = 
{
	// Positions			// Normals			// TexCoords
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// Bottom Left
	 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		// Bottom Right
	 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		// Top Right
	-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f		// Top Left
};

unsigned int g_QuadIndices[] =
{
	0, 1, 2,
	0, 2, 3
};


size_t g_QuadVerticesSizeInBytes = sizeof(g_QuadVertices);
size_t g_QuadVerticesWithNormalsSizeInBytes = sizeof(g_QuadVerticesWithNormals);
size_t g_TexturedQuadVerticesSizeInBytes = sizeof(g_TexturedQuadVertices);
size_t g_TexturedQuadVerticesWithNormalsSizeInBytes = sizeof(g_TexturedQuadVerticesWithNormals);

size_t g_QuadIndicesSizeInBytes = sizeof(g_QuadIndices);
