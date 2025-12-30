/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Sibur.cpp
*/

#include "Sibur.hpp"

namespace renderer3d
{
    IMesh *Sibur::mesh = nullptr;
    GLuint Sibur::textureId = 0;

    Sibur::Sibur(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Sibur::~Sibur()
    {
    }

    void Sibur::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Sibur::update(float deltaTime)
    {
    }

    std::string Sibur::getType() const
    {
        return "sibur";
    }

    void Sibur::getMineralType() const
    {
    }

    void Sibur::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Sibur::getPosition() const { return position; }

    void Sibur::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Sibur::getRotation() const { return rotation; }

    void Sibur::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Sibur::getScale() const { return scale; }

    void Sibur::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Sibur::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}
