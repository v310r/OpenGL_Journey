#pragma once

#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement 
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class FlyCamera
{
public:
    // constructor with vectors
    FlyCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = - 90.0f, float pitch = 0.0f);
    // constructor with scalar values
    FlyCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix() const;

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset);

    glm::vec3 GetPosition() const { return m_Position; }
    void SetPosition(glm::vec3 value) { m_Position = value; }
    void SetPosition(float x, float y, float z) { SetPosition({ x, y, z }); }

    float GetSpeed() const { return m_MovementSpeed; }
    void SetSpeed(float value) { m_MovementSpeed = value; }

    float GetZoom() const { return m_Zoom; }
    void SetZoom(float value) { m_Zoom = value; }

	float GetYaw() const
	{
		return m_Yaw;
	}

	void SetYaw(float value)
	{
		m_Yaw = value;
		UpdateCameraVectors();
	}

	float GetPitch() const
	{
		return m_Pitch;
	}

	void SetPitch(float value)
	{
		m_Pitch = value;
		UpdateCameraVectors();
	}

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void UpdateCameraVectors();

    // camera Attributes
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;
    // euler Angles
    float m_Yaw;
    float m_Pitch;
    // camera options
    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;
};