/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IRenderer.hpp
*/

#pragma once

class IRenderer
{
public:
    virtual ~IRenderer() = default;
    virtual void initialize() = 0;
    virtual void render() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void shutdown() = 0;
};
