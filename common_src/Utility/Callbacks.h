#pragma once

struct GLFWwindow;

namespace Callbacks
{
    void OnWindowSizeChanged(GLFWwindow* window, int width, int height);

    void OnMouseMoved(GLFWwindow* window, double xPos, double yPos);

    void OnMouseScrolled(GLFWwindow* window, double xOffset, double yOffset);

    void OnMouseButtonAction(GLFWwindow* window, int button, int action, int mods);
}