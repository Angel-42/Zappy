/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ISlider.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>

class ISlider {
public:
    virtual ~ISlider() = default;

    virtual void setPosition(const sf::Vector2f& pos) = 0;
    virtual void setSize(const sf::Vector2f& size) = 0;
    virtual void setValue(float value) = 0;
    virtual float getValue() const = 0;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void draw(sf::RenderTarget& target) const = 0;
};
