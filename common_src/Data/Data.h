#pragma once

#include "glm.hpp"


// TODO: Make compact version for cube; use 24 vertices instead of 36

extern float g_CubeVerticesWithNormals[];
extern float g_TexturedCubeVertices[];
extern float g_TexturedCubeVerticesWithNormals[];

extern size_t g_CubeVerticesWithNormalsSizeInBytes;
extern size_t g_TexturedCubeVerticesSizeInBytes;
extern size_t g_TexturedCubeVerticesWithNormalsSizeInBytes;

// Compact

extern float g_CompactTexturedCubeVertices[];
extern float g_CompactCubeVertices[];
extern unsigned int g_CubeIndices[];

extern size_t g_CompactTexturedCubeVerticesSizeInBytes;
extern size_t g_CompactCubeVerticesSizeInBytes;
extern size_t g_CubeIndicesSizeInBytes;


extern float g_QuadVertices[];
extern float g_QuadVerticesWithNormals[];
extern float g_TexturedQuadVertices[];
extern float g_TexturedQuadVerticesWithNormals[];
extern unsigned int g_QuadIndices[];

extern size_t g_QuadVerticesSizeInBytes;
extern size_t g_QuadVerticesWithNormalsSizeInBytes;
extern size_t g_TexturedQuadVerticesSizeInBytes;
extern size_t g_TexturedQuadVerticesWithNormalsSizeInBytes;

extern size_t g_QuadIndicesSizeInBytes;
