/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Grid.hpp
*/

#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

class Grid
{
public:
    explicit Grid(int width, int height, float tileSize = 1.0f, glm::vec3 position = glm::vec3(0.0f));
    ~Grid();

    void draw();
    glm::vec3 position;

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    float getTileSize() const { return tileSize; }

private:
    void generateGrid();

    int width, height;
    float tileSize;
    GLuint vao, vbo;
    size_t vertexCount;
    bool enabled;
};
