/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Deraumere.cpp
*/

#include "Deraumere.hpp"

namespace renderer3d
{
    IMesh *Deraumere::mesh = nullptr;
    GLuint Deraumere::textureId = 0;

    Deraumere::Deraumere(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Deraumere::~Deraumere()
    {
    }

    void Deraumere::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Deraumere::update(float deltaTime)
    {
    }

    std::string Deraumere::getType() const
    {
        return "deraumere";
    }

    void Deraumere::getMineralType() const
    {
    }

    void Deraumere::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Deraumere::getPosition() const { return position; }

    void Deraumere::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Deraumere::getRotation() const { return rotation; }

    void Deraumere::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Deraumere::getScale() const { return scale; }

    void Deraumere::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Deraumere::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}