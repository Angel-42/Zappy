/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** DaylightCycle.hpp
*/

#pragma once

#include <glm/glm.hpp>

class DaylightCycle
{
public:
    DaylightCycle();

    void update(float deltaTime);
    glm::vec3 getSunDirection() const;
    glm::vec3 getSunColor() const;
    float getTimeOfDay() const;

private:
    float m_timeOfDay;
    float m_cycleSpeed;
};
