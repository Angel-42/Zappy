/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Camera.hpp
*/

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;
    Camera(Camera &&) = default;
    Camera &operator=(Camera &&) = default;

    void processKeyboard(float deltaX, float deltaY, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void update(float dx, float dy, float deltaTime, float xoffset, float yoffset);

    [[nodiscard]] glm::mat4 getViewMatrix() const noexcept;
    [[nodiscard]] glm::vec3 getPosition() const noexcept;

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;

    void updateCameraVectors() noexcept;
};
