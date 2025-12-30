/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Thystame.hpp
*/

#pragma once

#include "../IMinerals.hpp"
#include "../../../IMesh.hpp"
#include <glm/glm.hpp>

namespace renderer3d
{
    class Thystame : public IMinerals
    {
    public:
        Thystame(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl);
        ~Thystame();

        void draw() override final;
        void update(float deltaTime) override final;
        std::string getType() const override final;
        void getMineralType() const override final;

        void setPosition(const glm::vec3 &pos);
        glm::vec3 getPosition() const;

        void setRotation(const glm::vec3 &rot);
        glm::vec3 getRotation() const;

        void setScale(const glm::vec3 &scl);
        glm::vec3 getScale() const;

        void move(const glm::vec3 &direction, float speed);
        void rotate(const glm::vec3 &axis, float angle);

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        static IMesh *mesh;
        static GLuint textureId;
    };
}
