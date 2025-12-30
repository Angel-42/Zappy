/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** CircleButton.cpp
*/

#include "CircleButton.hpp"

CircleButton::CircleButton(float radius, const std::string &text)
{
    m_shape.setRadius(radius);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(2.f);
    m_text.setString(text);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(20);
}

void CircleButton::setPosition(const sf::Vector2f &pos)
{
    m_shape.setPosition(pos);
    // Center text in circle
    sf::FloatRect textRect = m_text.getLocalBounds();
    float diameter = m_shape.getRadius() * 2.f;
    m_text.setPosition(
        pos.x + (diameter - textRect.width) / 2 - textRect.left,
        pos.y + (diameter - textRect.height) / 2 - textRect.top);
}

void CircleButton::setSize(const sf::Vector2f &size)
{
    m_shape.setRadius(size.x / 2.f);
    setPosition(m_shape.getPosition());
}

void CircleButton::setText(const std::string &text)
{
    m_text.setString(text);
    setPosition(m_shape.getPosition());
}

void CircleButton::setCallback(std::function<void()> callback)
{
    m_callback = std::move(callback);
}

void CircleButton::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
        sf::Vector2f center = m_shape.getPosition() + sf::Vector2f(m_shape.getRadius(), m_shape.getRadius());
        float dist = std::hypot(mouse.x - center.x, mouse.y - center.y);
        if (dist <= m_shape.getRadius())
        {
            if (m_callback)
                m_callback();
        }
    }
}

void CircleButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_text, states);
}
