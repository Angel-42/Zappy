/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Food.cpp
*/

#include "Food.hpp"

namespace renderer3d
{
    IMesh *Food::mesh = nullptr;
    GLuint Food::textureId = 0;

    Food::Food(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
        : position(pos), rotation(rot), scale(scl)
    {
    }

    Food::~Food()
    {
    }

    void Food::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Food::update(float deltaTime)
    {
    }

    std::string Food::getType() const
    {
        return "food";
    }

    void Food::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Food::getPosition() const { return position; }

    void Food::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Food::getRotation() const { return rotation; }

    void Food::setScale(const glm::vec3 &scl) { scale = scl; }
    glm::vec3 Food::getScale() const { return scale; }

    void Food::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Food::rotate(const glm::vec3 &axis, float angle)
    {
        rotation += axis * angle;
    }
}
