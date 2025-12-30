/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** WaterMesh.hpp
*/

#pragma once

#include "../IMesh.hpp"

class WaterMesh : public IMesh
{
public:
    void generate(int gridSize) override final;
    void uploadToGPU() override final;
    void draw() override final;
    void cleanup() override final;

private:
    std::vector<float> vertices;
    std::vector<GLuint> indices;
    GLuint vao, vbo, ebo;
};
