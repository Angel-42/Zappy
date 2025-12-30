/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Food.hpp
*/

#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../IRessources.hpp"
#include "../../../Objects/IMesh.hpp"

namespace renderer3d
{
    class Food : public IRessources
    {
    public:
        Food(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl);
        ~Food() override;

        void draw() override;
        void update(float deltaTime) override;

        void setPosition(const glm::vec3 &pos);
        glm::vec3 getPosition() const;

        void setRotation(const glm::vec3 &rot);
        glm::vec3 getRotation() const;

        void setScale(const glm::vec3 &scl);
        glm::vec3 getScale() const;

        std::string getType() const override;

        void move(const glm::vec3 &direction, float speed);
        void rotate(const glm::vec3 &axis, float angle);

        void setMesh(IMesh *m) { mesh = m; }
        IMesh *getMesh() const { return mesh; }

        void setTextureId(GLuint id) { textureId = id; }
        GLuint getTextureId() const { return textureId; }

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        static IMesh *mesh;
        static GLuint textureId;
    };
}
