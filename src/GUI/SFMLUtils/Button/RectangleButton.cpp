/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RectangleButton.cpp
*/

#include "RectangleButton.hpp"

RectangleButton::RectangleButton(const sf::Vector2f &size, const std::string &text)
{
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(2.f);
    m_text.setString(text);
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(20);
}

void RectangleButton::setPosition(const sf::Vector2f &pos)
{
    m_shape.setPosition(pos);
    // Center text in button
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setPosition(
        pos.x + (m_shape.getSize().x - textRect.width) / 2 - textRect.left,
        pos.y + (m_shape.getSize().y - textRect.height) / 2 - textRect.top);
}

void RectangleButton::setSize(const sf::Vector2f &size)
{
    m_shape.setSize(size);
    setPosition(m_shape.getPosition());
}

void RectangleButton::setText(const std::string &text)
{
    m_text.setString(text);
    setPosition(m_shape.getPosition());
}

void RectangleButton::setCallback(std::function<void()> callback)
{
    m_callback = std::move(callback);
}

void RectangleButton::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2f mouse(event.mouseButton.x, event.mouseButton.y);
        if (m_shape.getGlobalBounds().contains(mouse))
        {
            if (m_callback)
                m_callback();
        }
    }
}

void RectangleButton::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_shape, states);
    target.draw(m_text, states);
}
