/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Tile3D.hpp
*/

#pragma once

#include "ITile.hpp"
#include "../Utils/Shape3D.hpp"
#include <memory>

class Tile3D : public ITile
{
public:
    Tile3D(const glm::vec3 &pos, std::shared_ptr<Shape3D> shape);

    void setPosition(const glm::vec3 &pos) override;
    const glm::vec3 &getPosition() const override;
    void draw() override;

private:
    glm::vec3 _position;
    std::shared_ptr<Shape3D> _shape;
};
