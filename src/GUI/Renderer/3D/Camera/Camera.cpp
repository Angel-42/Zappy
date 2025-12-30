/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Camera.cpp
*/

#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position(position), worldUp(up), yaw(yaw), pitch(pitch)
{
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const noexcept
{
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(float deltaX, float deltaY, float deltaTime)
{
    static constexpr float velocityBase = 2.5f;
    float velocity = velocityBase * deltaTime;
    position += front * deltaY * velocity;
    position += right * deltaX * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    static constexpr float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) [[unlikely]]
        pitch = 89.0f;
    if (pitch < -89.0f) [[unlikely]]
        pitch = -89.0f;

    updateCameraVectors();
}

glm::vec3 Camera::getPosition() const noexcept
{
    return position;
}

void Camera::updateCameraVectors() noexcept
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::update(float dx, float dy, float deltaTime, float xoffset, float yoffset)
{
    processKeyboard(dx, dy, deltaTime);
    processMouseMovement(xoffset, yoffset);
}
