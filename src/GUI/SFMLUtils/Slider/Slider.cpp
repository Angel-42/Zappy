/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Slider.cpp
*/

#include "Slider.hpp"
#include <algorithm>

Slider::Slider(float minValue, float maxValue, float initialValue)
    : m_min(minValue), m_max(maxValue), m_value(initialValue)
{
    m_bar.setFillColor(sf::Color(180, 180, 180));
    m_knob.setRadius(10.f);
    m_knob.setOrigin(10.f, 10.f);
    m_knob.setFillColor(sf::Color(100, 100, 255));
    setSize({150.f, 6.f});
    setPosition({0.f, 0.f});
    setValue(initialValue);
}

void Slider::setPosition(const sf::Vector2f &pos)
{
    m_position = pos;
    m_bar.setPosition(pos);
    m_knob.setPosition({valueToX(m_value), pos.y + m_size.y / 2});
}

void Slider::setSize(const sf::Vector2f &size)
{
    m_size = size;
    m_bar.setSize(size);
    m_bar.setOrigin(0, size.y / 2);
    m_knob.setPosition({valueToX(m_value), m_position.y + size.y / 2});
}

void Slider::setValue(float value)
{
    m_value = std::clamp(value, m_min, m_max);
    m_knob.setPosition({valueToX(m_value), m_position.y + m_size.y / 2});
    if (m_callback)
        m_callback(m_value);
}

float Slider::getValue() const
{
    return m_value;
}

void Slider::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
        if (m_knob.getGlobalBounds().contains(mouse))
        {
            m_dragging = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        m_dragging = false;
    }
    else if (event.type == sf::Event::MouseMoved && m_dragging)
    {
        float x = static_cast<float>(event.mouseMove.x);
        float left = m_position.x;
        float right = m_position.x + m_size.x;
        x = std::clamp(x, left, right);
        setValue(xToValue(x));
    }
}

void Slider::draw(sf::RenderTarget &target) const
{
    target.draw(m_bar);
    target.draw(m_knob);
}

void Slider::setCallback(std::function<void(float)> callback)
{
    m_callback = std::move(callback);
}

float Slider::valueToX(float value) const
{
    float t = (value - m_min) / (m_max - m_min);
    return m_position.x + t * m_size.x;
}

float Slider::xToValue(float x) const
{
    float t = (x - m_position.x) / m_size.x;
    return m_min + t * (m_max - m_min);
}
