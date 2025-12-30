/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GLShapeFactory.hpp
*/

#pragma once
#include "Shape3D.hpp"

class ShapeFactory
{
public:
    static Shape3D createCube();
    static Shape3D createGrid(int size);
    static Shape3D createCircle(float radius, int segments);
};
