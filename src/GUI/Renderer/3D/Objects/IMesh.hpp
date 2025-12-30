/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IMesh.hpp
*/

#pragma once

#include <vector>
#include <GL/glew.h>

class IMesh
{
public:
    virtual ~IMesh() = default;

    virtual void generate(int gridSize) = 0;
    virtual void uploadToGPU() = 0;
    virtual void draw() = 0;
    virtual void cleanup() = 0;
};
