/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ProceduralTextureGenerator.cpp
*/

#include "ProceduralTextureGenerator.hpp"
#include <vector>
#include <cmath>

GLuint ProceduralTextureGenerator::createDummyTexture(int r, int g, int b)
{
    const int width = 256, height = 256;
    std::vector<unsigned char> data(width * height * 4);

    for (int i = 0; i < width * height; i++)
    {
        data[i * 4 + 0] = r;
        data[i * 4 + 1] = g;
        data[i * 4 + 2] = b;
        data[i * 4 + 3] = 255;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return tex;
}

GLuint ProceduralTextureGenerator::createProceduralDudvMap(int width, int height)
{
    std::vector<unsigned char> data(width * height * 4);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char r = 128 + (unsigned char)(127.0f * sin(i * 0.1f) * cos(j * 0.1f));
            unsigned char g = 128 + (unsigned char)(127.0f * sin(j * 0.15f + 0.5f) * cos(i * 0.12f));
            int idx = (i * width + j) * 4;
            data[idx + 0] = r;
            data[idx + 1] = g;
            data[idx + 2] = 255;
            data[idx + 3] = 255;
        }
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return tex;
}

GLuint ProceduralTextureGenerator::createProceduralCartoonWater(int width, int height)
{
    std::vector<unsigned char> data(width * height * 4);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float pattern = sin(j * 0.1f) * cos(i * 0.1f) * 0.5f + 0.5f;
            pattern *= sin(i * 0.05f + j * 0.05f) * 0.3f + 0.7f;
            unsigned char blue = static_cast<unsigned char>(180 + 75 * pattern);
            unsigned char green = static_cast<unsigned char>(130 + 60 * pattern);
            unsigned char red = static_cast<unsigned char>(30 + 40 * pattern);
            int idx = (i * width + j) * 4;
            data[idx + 0] = red;
            data[idx + 1] = green;
            data[idx + 2] = blue;
            data[idx + 3] = 255;
        }
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return tex;
}
