#include "Callbacks.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera/FlyCamera.h"


extern int g_WindowWidth;
extern int g_WindowHeight;

extern float g_DeltaTime;

extern float g_MouseLastX;
extern float g_MouseLastY;

extern bool g_bFirstMouseInput;

static bool s_bRightMouseButtonPressed = false;

extern FlyCamera g_Camera;

void Callbacks::OnWindowSizeChanged(GLFWwindow* window, int width, int height)
{
    g_WindowWidth = width;
    g_WindowHeight = height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Callbacks::OnMouseMoved(GLFWwindow* window, double xPos, double yPos)
{
    if (!s_bRightMouseButtonPressed)
    {
        return;
    }

    if (g_bFirstMouseInput)
    {
        g_MouseLastX = static_cast<float>(xPos);
        g_MouseLastY = static_cast<float>(yPos);
        g_bFirstMouseInput = false;
    }

    float xOffset = static_cast<float>(xPos) - g_MouseLastX;
    float yOffset = static_cast<float>(yPos) - g_MouseLastY;

    g_MouseLastX = static_cast<float>(xPos);
    g_MouseLastY = static_cast<float>(yPos);

    g_Camera.ProcessMouseMovement(xOffset, yOffset);
}

void Callbacks::OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset)
{
    g_Camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void Callbacks::OnMouseButtonAction(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        s_bRightMouseButtonPressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        s_bRightMouseButtonPressed = false;
    }
}
