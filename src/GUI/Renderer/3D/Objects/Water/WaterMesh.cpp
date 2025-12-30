/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** WaterMesh.cpp
*/

#include "WaterMesh.hpp"
#include <iostream>
#include <cmath>

void WaterMesh::generate(int gridSize)
{
    if (gridSize <= 0) [[unlikely]]
    {
        std::cerr << "Invalid grid size: " << gridSize << ". Must be greater than 0." << std::endl;
        return;
    }

    vertices.clear();
    indices.clear();

    constexpr float waterLevel = -0.05f;
    constexpr float waterSize = 30.0f;
    const int waterResolution = gridSize;
    const float waterStep = 2.0f * waterSize / waterResolution;
    constexpr float waterTiling = 30.0f;

    vertices.reserve((waterResolution + 1) * (waterResolution + 1) * 6);
    indices.reserve(waterResolution * waterResolution * 6);

    for (std::size_t i = 0; i <= static_cast<std::size_t>(waterResolution); ++i)
    {
        for (std::size_t j = 0; j <= static_cast<std::size_t>(waterResolution); ++j)
        {
            float x = -waterSize + j * waterStep;
            float z = -waterSize + i * waterStep;
            vertices.push_back(x);
            vertices.push_back(waterLevel);
            vertices.push_back(z);
            float u = static_cast<float>(j) / waterResolution * waterTiling;
            float v = static_cast<float>(i) / waterResolution * waterTiling;

            float noiseU = 0.1f * sin(j * 0.37f + i * 0.21f);
            float noiseV = 0.1f * cos(j * 0.41f + i * 0.29f);
            u += noiseU;
            v += noiseV;

            vertices.push_back(u);
            vertices.push_back(v);
            vertices.push_back(1.0f); // Alpha value for water
        }
    }
    for (std::size_t i = 0; i < static_cast<std::size_t>(waterResolution); ++i)
    {
        for (std::size_t j = 0; j < static_cast<std::size_t>(waterResolution); ++j)
        {
            std::size_t topLeft = i * (waterResolution + 1) + j;
            std::size_t topRight = topLeft + 1;
            std::size_t bottomLeft = topLeft + (waterResolution + 1);
            std::size_t bottomRight = bottomLeft + 1;

            indices.push_back(static_cast<GLuint>(topLeft));
            indices.push_back(static_cast<GLuint>(bottomLeft));
            indices.push_back(static_cast<GLuint>(topRight));

            indices.push_back(static_cast<GLuint>(topRight));
            indices.push_back(static_cast<GLuint>(bottomLeft));
            indices.push_back(static_cast<GLuint>(bottomRight));
        }
    }
}

void WaterMesh::uploadToGPU()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void WaterMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void WaterMesh::cleanup()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
