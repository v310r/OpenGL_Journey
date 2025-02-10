#include "FlyCamera.h"

// Default camera values
constexpr float CAMERA_DEFAULT_SPEED = 5.0f;
constexpr float CAMERA_DEFAULT_SENSITIVITY = 0.1f;
constexpr float CAMERA_DEFAULT_ZOOM = 45.0f;


FlyCamera::FlyCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(CAMERA_DEFAULT_SPEED), m_MouseSensitivity(CAMERA_DEFAULT_SENSITIVITY), m_Zoom(CAMERA_DEFAULT_ZOOM)
{
    m_Position = position;
    m_WorldUp = up;
    m_Yaw = yaw;
    m_Pitch = pitch;
    UpdateCameraVectors();
}

FlyCamera::FlyCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(CAMERA_DEFAULT_SPEED), m_MouseSensitivity(CAMERA_DEFAULT_SENSITIVITY), m_Zoom(CAMERA_DEFAULT_ZOOM)
{
    m_Position = glm::vec3(posX, posY, posZ);
    m_WorldUp = glm::vec3(upX, upY, upZ);
    m_Yaw = yaw;
    m_Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 FlyCamera::GetViewMatrix()
{
    glm::vec3 frontVector = glm::normalize(m_Position - (m_Position + m_Front));
    glm::vec3 rightVector = glm::normalize(glm::cross(m_Up, frontVector));
    glm::vec3 upVector = glm::normalize(glm::cross(frontVector, rightVector));

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3][0] = -m_Position.x;
    translation[3][1] = -m_Position.y;
    translation[3][2] = -m_Position.z;


    glm::mat4 rotation = glm::mat4(1.0f);
    rotation[0][0] = rightVector.x; // First column, first row
    rotation[1][0] = rightVector.y;
    rotation[2][0] = rightVector.z;
    rotation[0][1] = upVector.x; // First column, second row
    rotation[1][1] = upVector.y;
    rotation[2][1] = upVector.z;
    rotation[0][2] = frontVector.x; // First column, third row
    rotation[1][2] = frontVector.y;
    rotation[2][2] = frontVector.z;

    //rotation[0][0] = rightVector.x; // First column, first row
    //rotation[0][1] = rightVector.y;
    //rotation[0][2] = rightVector.z;
    //rotation[1][0] = upVector.x; // First column, second row
    //rotation[1][1] = upVector.y;
    //rotation[1][2] = upVector.z;
    //rotation[2][0] = frontVector.x; // First column, third row
    //rotation[2][1] = frontVector.y;
    //rotation[2][2] = frontVector.z;

    return rotation * translation;

    //return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void FlyCamera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_MovementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD)
    {
        m_Position += m_Front * velocity;
    }

    if (direction == CameraMovement::BACKWARD)
    {
        m_Position -= m_Front * velocity;
    }

    if (direction == CameraMovement::LEFT)
    {
        m_Position -= m_Right * velocity;
    }

    if (direction == CameraMovement::RIGHT)
    {
        m_Position += m_Right * velocity;
    }

    if (direction == CameraMovement::UP)
    {
        m_Position.y += velocity;
    }

    if (direction == CameraMovement::DOWN)
    {
        m_Position.y -=  velocity;
    }
}

void FlyCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch -= yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    UpdateCameraVectors();
}

void FlyCamera::ProcessMouseScroll(float yoffset)
{
    m_Zoom -= yoffset;
    if (m_Zoom < 1.0f)
        m_Zoom = 1.0f;
    if (m_Zoom > 45.0f)
        m_Zoom = 45.0f;
}

void FlyCamera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
