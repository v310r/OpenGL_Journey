#pragma once

#include "glad/glad.h"

namespace Debug
{
    void OnOpenGLdebugCallback(
        GLenum source, 
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length, 
        const GLchar* 
        message, 
        const void* userParam);

  
}