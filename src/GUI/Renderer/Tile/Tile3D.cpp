/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Tile3D.cpp
*/

#include "Tile3D.hpp"

Tile3D::Tile3D(const glm::vec3 &pos, std::shared_ptr<Shape3D> shape)
    : _position(pos), _shape(std::move(shape))
{
}

void Tile3D::setPosition(const glm::vec3 &pos)
{
    _position = pos;
}

const glm::vec3 &Tile3D::getPosition() const
{
    return _position;
}

void Tile3D::draw()
{
    if (_shape)
        _shape->draw();
}
