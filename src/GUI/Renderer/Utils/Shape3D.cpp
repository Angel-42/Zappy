/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Shape3D.cpp
*/

#include "Shape3D.hpp"

Shape3D::Shape3D(GLuint vao, GLuint vbo, GLuint ebo, GLsizei indexCount)
    : _vao(vao), _vbo(vbo), _ebo(ebo), _indexCount(indexCount)
{
}

Shape3D::~Shape3D()
{
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);
}

void Shape3D::draw() const
{
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
