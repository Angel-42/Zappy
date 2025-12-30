/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Linemate.hpp
*/

#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../IMinerals.hpp"
#include "../../../../Objects/IMesh.hpp"

namespace renderer3d
{
    class Linemate : public IMinerals
    {
    public:
        Linemate(const glm::vec3 &pos = glm::vec3(0.0f),
                 const glm::vec3 &rot = glm::vec3(0.0f),
                 const glm::vec3 &scl = glm::vec3(1.0f));
        ~Linemate();

        void draw() override;
        void update(float deltaTime) override;
        std::string getType() const override;
        void getMineralType() const override;

        void setPosition(const glm::vec3 &pos);
        glm::vec3 getPosition() const;

        void setRotation(const glm::vec3 &rot);
        glm::vec3 getRotation() const;

        void setScale(const glm::vec3 &scl);
        glm::vec3 getScale() const;

        void move(const glm::vec3 &direction, float speed);
        void rotate(const glm::vec3 &axis, float angle);

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

    public:
        static IMesh *mesh;
        static GLuint textureId;
    };
}