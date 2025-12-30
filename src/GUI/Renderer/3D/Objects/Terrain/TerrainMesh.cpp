/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** TerrainMesh.cpp
*/

#include "TerrainMesh.hpp"
#include <cmath>
#include <ranges>
#include <cstddef>
#include <iostream>

void TerrainMesh::generate(int gridSize)
{
    if (gridSize <= 0) [[unlikely]]
    {
        std::cerr << "Invalid grid size: " << gridSize << ". Must be greater than 0." << std::endl;
        return;
    }

    vertices.clear();
    indices.clear();

    constexpr float bottomY = -0.05f;
    constexpr int floatsPerVertex = 6;

    // Top face
    for (std::size_t i : std::views::iota(0, gridSize + 1))
    {
        for (std::size_t j : std::views::iota(0, gridSize + 1))
        {
            float x = static_cast<float>(j) / gridSize * 2.0f - 1.0f;
            float z = static_cast<float>(i) / gridSize * 2.0f - 1.0f;
            float height = 0.0f;
            vertices.push_back(x);
            vertices.push_back(height);
            vertices.push_back(z);
            vertices.push_back(static_cast<float>(j) / gridSize * 3.0f);
            vertices.push_back(static_cast<float>(i) / gridSize * 3.0f);
            vertices.push_back(height);
        }
    }
    int topOffset = 0;
    for (std::size_t i : std::views::iota(0, gridSize))
    {
        for (std::size_t j : std::views::iota(0, gridSize))
        {
            int topLeft = topOffset + i * (gridSize + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (gridSize + 1);
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    int bottomOffset = vertices.size() / 6;
    for (std::size_t i : std::views::iota(0, gridSize + 1))
    {
        for (std::size_t j : std::views::iota(0, gridSize + 1))
        {
            float x = static_cast<float>(j) / gridSize * 2.0f - 1.0f;
            float z = static_cast<float>(i) / gridSize * 2.0f - 1.0f;
            vertices.push_back(x);
            vertices.push_back(bottomY);
            vertices.push_back(z);
            vertices.push_back(static_cast<float>(j) / gridSize * 3.0f);
            vertices.push_back(static_cast<float>(i) / gridSize * 3.0f);
            vertices.push_back(bottomY);
        }
    }
    for (std::size_t i : std::views::iota(0, gridSize))
    {
        for (std::size_t j : std::views::iota(0, gridSize))
        {
            int topLeft = bottomOffset + i * (gridSize + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (gridSize + 1);
            int bottomRight = bottomLeft + 1;
            indices.push_back(topLeft);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(bottomRight);
            indices.push_back(bottomLeft);
        }
    }

    int sideOffset = vertices.size() / floatsPerVertex;

    // Lambda -> Generate side vertices and indices
    auto addSide = [&](auto getPos, int sideCount)
    {
        for (int i = 0; i < sideCount; ++i)
        {
            for (std::size_t k : std::views::iota(0, gridSize + 1))
            {
                auto [x, z] = getPos(i, k);
                float yTop = 0.0f;
                float yBottom = bottomY;
                vertices.insert(vertices.end(), {x, yTop, z, k / static_cast<float>(gridSize) * 3.0f, 0.0f, yTop});
                vertices.insert(vertices.end(), {x, yBottom, z, k / static_cast<float>(gridSize) * 3.0f, 0.0f, yBottom});
            }
            for (std::size_t k : std::views::iota(0, gridSize))
            {
                int base = sideOffset + i * (gridSize + 1) * 2 + k * 2;
                indices.push_back(base);
                indices.push_back(base + 2);
                indices.push_back(base + 1);

                indices.push_back(base + 1);
                indices.push_back(base + 2);
                indices.push_back(base + 3);
            }
        }
        sideOffset += (gridSize + 1) * 2 * sideCount;
    };

    // +x side (j = gridSize)
    addSide(
        [gridSize](int i, int k)
        {
            return std::pair<float, float>{1.0f, static_cast<float>(k) / gridSize * 2.0f - 1.0f};
        },
        gridSize);

    // -x side (j = 0)
    addSide(
        [gridSize](int i, int k)
        {
            return std::pair<float, float>{-1.0f, static_cast<float>(k) / gridSize * 2.0f - 1.0f};
        },
        gridSize);

    // +z side (i = gridSize)
    addSide(
        [gridSize](int j, int k)
        {
            return std::pair<float, float>{static_cast<float>(k) / gridSize * 2.0f - 1.0f, 1.0f};
        },
        gridSize);

    // -z side (i = 0)
    addSide(
        [gridSize](int j, int k)
        {
            return std::pair<float, float>{static_cast<float>(k) / gridSize * 2.0f - 1.0f, -1.0f};
        },
        gridSize);
}

void TerrainMesh::uploadToGPU()
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

void TerrainMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void TerrainMesh::cleanup()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}
