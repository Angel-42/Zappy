/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Mendiane.cpp
*/

#include "Mendiane.hpp"

namespace renderer3d
{
    IMesh *Mendiane::mesh = nullptr;
    GLuint Mendiane::textureId = 0;

    Mendiane::Mendiane(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Mendiane::~Mendiane()
    {
    }

    void Mendiane::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Mendiane::update(float deltaTime)
    {
    }

    std::string Mendiane::getType() const
    {
        return "mendiane";
    }

    void Mendiane::getMineralType() const
    {
    }

    void Mendiane::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Mendiane::getPosition() const { return position; }

    void Mendiane::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Mendiane::getRotation() const { return rotation; }

    void Mendiane::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Mendiane::getScale() const { return scale; }

    void Mendiane::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Mendiane::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}
