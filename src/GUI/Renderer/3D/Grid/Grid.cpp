/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Grid.cpp
*/

#include "Grid.hpp"
#include <ranges>

Grid::Grid(int w, int h, float t, glm::vec3 pos)
    : width(w), height(h), tileSize(t), vao(0), vbo(0), vertexCount(0), position(pos), enabled(true)
{
    generateGrid();
}

Grid::~Grid()
{
    if (vao)
        glDeleteVertexArrays(1, &vao);
    if (vbo)
        glDeleteBuffers(1, &vbo);
}

void Grid::generateGrid()
{
    constexpr std::size_t floatsPerVertex = 6;
    std::vector<float> vertices;
    vertices.reserve(static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 6 * floatsPerVertex);
    for (std::size_t x : std::views::iota(std::size_t{0}, static_cast<std::size_t>(width)))
    {
        for (std::size_t z : std::views::iota(std::size_t{0}, static_cast<std::size_t>(height)))
        {
            const float color = ((x + z) % 2 == 0) ? 1.0f : 0.0f;
            const float x0 = static_cast<float>(x) * tileSize;
            const float z0 = static_cast<float>(z) * tileSize;
            const float x1 = static_cast<float>(x + 1) * tileSize;
            const float z1 = static_cast<float>(z + 1) * tileSize;

            // Two triangles per tile:
            // -> Triangle 1
            vertices.insert(vertices.end(), {x0, 0, z0, color, color, color});
            vertices.insert(vertices.end(), {x1, 0, z0, color, color, color});
            vertices.insert(vertices.end(), {x1, 0, z1, color, color, color});
            // -> Triangle 2
            vertices.insert(vertices.end(), {x0, 0, z0, color, color, color});
            vertices.insert(vertices.end(), {x1, 0, z1, color, color, color});
            vertices.insert(vertices.end(), {x0, 0, z1, color, color, color});
        }
    }
    vertexCount = vertices.size() / floatsPerVertex;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floatsPerVertex * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Grid::draw()
{
    if (!enabled || vertexCount == 0)
        return;
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);
}