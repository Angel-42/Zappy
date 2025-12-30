/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** DaylightCycle.cpp
*/

#include "DaylightCycle.hpp"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

DaylightCycle::DaylightCycle()
    : m_timeOfDay(0.25f), m_cycleSpeed(0.02f)
{
}

void DaylightCycle::update(float deltaTime)
{
    m_timeOfDay += m_cycleSpeed * deltaTime;
    if (m_timeOfDay > 1.0f)
        m_timeOfDay -= 1.0f;
}

glm::vec3 DaylightCycle::getSunDirection() const
{
    float angle = m_timeOfDay * glm::two_pi<float>();
    return glm::normalize(glm::vec3(std::cos(angle), std::sin(angle), 0.2f));
}

glm::vec3 DaylightCycle::getSunColor() const
{
    float height = getSunDirection().y;
    glm::vec3 dayColor(1.0f, 0.95f, 0.85f);
    glm::vec3 nightColor(0.1f, 0.1f, 0.2f);
    float t = glm::clamp((height + 0.1f) / 1.1f, 0.0f, 1.0f);
    return glm::mix(nightColor, dayColor, t);
}

float DaylightCycle::getTimeOfDay() const
{
    return m_timeOfDay;
}

/*
Usage :

glm::vec3 sunDir = daylightCycle.getSunDirection();
glm::vec3 sunColor = daylightCycle.getSunColor();
glUniform3fv(glGetUniformLocation(shader->id(), "sunDirection"), 1, &sunDir[0]);
glUniform3fv(glGetUniformLocation(shader->id(), "sunColor"), 1, &sunColor[0]);

In the shader, you can use these uniforms to set the
sun's direction and color for lighting calculations.
*/