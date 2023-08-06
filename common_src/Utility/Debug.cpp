#include "Debug.h"

#include <iostream>


#define ENABLE_SEVERITY_HIGH         1
#define ENABLE_SEVERITY_MEDIUM       1
#define ENABLE_SEVERITY_LOW          1
#define ENABLE_SEVERITY_NOTIFICATION 0



static void PrintOpenGLDebugMessage(GLenum source, GLenum type, GLuint id, const GLchar* severity, const GLchar* message)
{
    std::cerr << "OpenGL Debug Message:\n";
    std::cerr << "Source: " << source << " | Type: " << type << " | ID: " << id << " | Severity: " << severity << "\n";
    std::cerr << "Message: " << message << "\n";
    std::cerr << "--------------------------\n";
}

void Debug::OnOpenGLdebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity)
    {
#if ENABLE_SEVERITY_HIGH == 1
        case GL_DEBUG_SEVERITY_HIGH:
        {
            PrintOpenGLDebugMessage(source, type, id, "GL_DEBUG_SEVERITY_HIGH", message);
            return;
        }
#endif
#if ENABLE_SEVERITY_MEDIUM == 1
        case GL_DEBUG_SEVERITY_MEDIUM:
        {
            PrintOpenGLDebugMessage(source, type, id, "GL_DEBUG_SEVERITY_MEDIUM", message);
            return;
        }
#endif
#if ENABLE_SEVERITY_LOW == 1
        case GL_DEBUG_SEVERITY_LOW:
        {
            PrintOpenGLDebugMessage(source, type, id, "GL_DEBUG_SEVERITY_LOW", message);
            return;
        }
#endif
#if ENABLE_SEVERITY_NOTIFICATION == 1
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        {
            PrintOpenGLDebugMessage(source, type, id, "GL_DEBUG_SEVERITY_NOTIFICATION", message);
            return;
        }
#endif

        default: return;
    }
}

