/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Linemate.cpp
*/

#include "Linemate.hpp"

namespace renderer3d
{
    IMesh *Linemate::mesh = nullptr;
    GLuint Linemate::textureId = 0;

    Linemate::Linemate(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Linemate::~Linemate()
    {
    }

    void Linemate::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Linemate::update(float deltaTime)
    {
    }

    std::string Linemate::getType() const
    {
        return "linemate";
    }

    void Linemate::getMineralType() const
    {
    }

    void Linemate::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Linemate::getPosition() const { return position; }

    void Linemate::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Linemate::getRotation() const { return rotation; }

    void Linemate::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Linemate::getScale() const { return scale; }

    void Linemate::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Linemate::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}
