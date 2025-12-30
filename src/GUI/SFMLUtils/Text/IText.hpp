/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IText.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>

class IText
{
public:
    virtual ~IText() = default;

    virtual void setPosition(const sf::Vector2f &pos) = 0;
    virtual void setString(const std::string &str) = 0;
    virtual void setFont(const sf::Font &font) = 0;
    virtual void setCharacterSize(unsigned int size) = 0;
    virtual void setFillColor(const sf::Color &color) = 0;

    virtual void draw(sf::RenderTarget &target) const = 0;
};
