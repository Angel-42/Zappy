/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ITile.hpp
*/

#pragma once

#include <glm/glm.hpp>

class ITile
{
public:
    virtual ~ITile() = default;
    virtual void setPosition(const glm::vec3 &pos) = 0;
    virtual const glm::vec3 &getPosition() const = 0;
    virtual void draw() = 0;
};
