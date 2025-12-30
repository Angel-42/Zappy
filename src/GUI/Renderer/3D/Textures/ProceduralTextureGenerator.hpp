/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ProceduralTextureGenerator.hpp
*/

#pragma once

#include <GL/glew.h>

class ProceduralTextureGenerator
{
public:
    static GLuint createDummyTexture(int r, int g, int b);
    static GLuint createProceduralDudvMap(int width = 256, int height = 256);
    static GLuint createProceduralCartoonWater(int width = 256, int height = 256);
};
