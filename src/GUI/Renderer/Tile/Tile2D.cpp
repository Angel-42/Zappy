/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Tile2D.cpp
*/

#include "Tile2D.hpp"

Tile2D::Tile2D(const glm::vec3 &pos, const sf::Vector2f &size, const sf::Color &color)
    : _position(pos)
{
    _shape.setSize(size);
    _shape.setFillColor(color);
    _shape.setPosition(pos.x, pos.y);
}

void Tile2D::setPosition(const glm::vec3 &pos)
{
    _position = pos;
    _shape.setPosition(pos.x, pos.y);
}

const glm::vec3 &Tile2D::getPosition() const
{
    return _position;
}

void Tile2D::setRenderTarget(sf::RenderTarget *target)
{
    _target = target;
}

void Tile2D::draw()
{
    if (_target)
        _target->draw(_shape);
}
