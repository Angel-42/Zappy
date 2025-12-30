/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IRessources.hpp
*/

#pragma once

#include <string>

class IRessources
{
public:
    virtual ~IRessources() = default;
    virtual void draw() = 0;
    virtual void update(float deltaTime) = 0;
    virtual std::string getType() const = 0;
};
