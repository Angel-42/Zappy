/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GLShapeFactory.cpp
*/

#include "GLShapeFactory.hpp"

Shape3D ShapeFactory::createCube()
{
    return Shape3D(0, 0, 0, 0);
}
Shape3D ShapeFactory::createGrid(int size)
{
    return Shape3D(0, 0, 0, 0);
}

Shape3D ShapeFactory::createCircle(float radius, int segments)
{
    return Shape3D(0, 0, 0, 0);
}
