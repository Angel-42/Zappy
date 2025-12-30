/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Text.cpp
*/

#include "Text.hpp"

Text::Text()
    : _text()
{
}

void Text::setPosition(const sf::Vector2f &pos)
{
    _text.setPosition(pos);
}

void Text::setString(const std::string &str)
{
    _text.setString(str);
}

void Text::setFont(const sf::Font &font)
{
    _text.setFont(font);
}

void Text::setCharacterSize(unsigned int size)
{
    _text.setCharacterSize(size);
}

void Text::setFillColor(const sf::Color &color)
{
    _text.setFillColor(color);
}

void Text::draw(sf::RenderTarget &target) const
{
    target.draw(_text);
}
