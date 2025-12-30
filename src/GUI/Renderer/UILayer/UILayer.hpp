/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** UILayer.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class UILayer
{
public:
    UILayer() = default;
    ~UILayer() = default;

    void addElement(const std::shared_ptr<sf::Drawable> &element);
    void draw(sf::RenderTarget &target) const;
    void handleEvent(const sf::Event &event);
    void initialize(sf::RenderWindow &window);
    void clear();

private:
    std::vector<std::shared_ptr<sf::Drawable>> m_elements;
};
