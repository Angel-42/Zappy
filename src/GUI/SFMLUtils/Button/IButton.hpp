/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IButton.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class IButton
{
public:
    virtual ~IButton() = default;

    virtual void setPosition(const sf::Vector2f &pos) = 0;
    virtual void setSize(const sf::Vector2f &size) = 0;
    virtual void setText(const std::string &text) = 0;
    virtual void setCallback(std::function<void()> callback) = 0;

    virtual void handleEvent(const sf::Event &event) = 0;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
};
