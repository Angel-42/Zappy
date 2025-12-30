/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Shape3D.hpp
*/

#pragma once
#include <GL/glew.h>

class Shape3D
{
public:
    Shape3D(GLuint vao, GLuint vbo, GLuint ebo, GLsizei indexCount);
    ~Shape3D();

    void draw() const;

private:
    GLuint _vao, _vbo, _ebo;
    GLsizei _indexCount;
};
