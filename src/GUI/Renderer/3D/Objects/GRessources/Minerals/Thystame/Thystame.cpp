/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Thystame.cpp
*/

#include "Thystame.hpp"

namespace renderer3d
{
    IMesh *Thystame::mesh = nullptr;
    GLuint Thystame::textureId = 0;

    Thystame::Thystame(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Thystame::~Thystame()
    {
    }

    void Thystame::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Thystame::update(float deltaTime)
    {
    }

    std::string Thystame::getType() const
    {
        return "thystame";
    }

    void Thystame::getMineralType() const
    {
    }

    void Thystame::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Thystame::getPosition() const { return position; }

    void Thystame::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Thystame::getRotation() const { return rotation; }

    void Thystame::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Thystame::getScale() const { return scale; }

    void Thystame::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Thystame::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}
