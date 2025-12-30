/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** UILayer.cpp
*/

#include "UILayer.hpp"

void UILayer::addElement(const std::shared_ptr<sf::Drawable> &element)
{
    if (!element)
    {
        throw std::invalid_argument("Cannot add a null element to UILayer");
    }
    m_elements.push_back(element);
}

void UILayer::draw(sf::RenderTarget &target) const
{
    for (const auto &elem : m_elements)
    {
        target.draw(*elem);
    }
}

void UILayer::handleEvent(const sf::Event &event)
{
}

void UILayer::initialize(sf::RenderWindow &window)
{
    // Initialization logic for the UILayer, if needed
    // For example, setting up fonts, loading textures, etc.
    // Currently, this method is empty as no specific initialization is required.
}

void UILayer::clear()
{
    m_elements.clear();
}
