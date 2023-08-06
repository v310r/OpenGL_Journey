#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

struct GLFWwindow;

class ImGuiWrapper
{
public:
    static void Init(GLFWwindow* window, const char* glslVersion);

    static void NewFrame();

    static void EndFrameAndRender();

    static void Cleanup();

    static ImGuiIO* GetIO();

    static ImGuiIO* s_IO;
};
