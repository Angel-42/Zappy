/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Phiras.cpp
*/

#include "Phiras.hpp"

namespace renderer3d
{
    IMesh *Phiras::mesh = nullptr;
    GLuint Phiras::textureId = 0;

    Phiras::Phiras(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Phiras::~Phiras()
    {
    }

    void Phiras::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Phiras::update(float deltaTime)
    {
    }

    std::string Phiras::getType() const
    {
        return "phiras";
    }

    void Phiras::getMineralType() const
    {
    }

    void Phiras::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Phiras::getPosition() const { return position; }

    void Phiras::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Phiras::getRotation() const { return rotation; }

    void Phiras::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Phiras::getScale() const { return scale; }

    void Phiras::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Phiras::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}
