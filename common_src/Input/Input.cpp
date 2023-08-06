#include "Input.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm.hpp"

#include "Camera/FlyCamera.h"


extern FlyCamera g_Camera;

extern float g_DeltaTime;

void Input::Process(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        g_Camera.ProcessKeyboard(CameraMovement::FORWARD, g_DeltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        g_Camera.ProcessKeyboard(CameraMovement::BACKWARD, g_DeltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        g_Camera.ProcessKeyboard(CameraMovement::LEFT, g_DeltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        g_Camera.ProcessKeyboard(CameraMovement::RIGHT, g_DeltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        g_Camera.ProcessKeyboard(CameraMovement::DOWN, g_DeltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        g_Camera.ProcessKeyboard(CameraMovement::UP, g_DeltaTime);
    }
}

