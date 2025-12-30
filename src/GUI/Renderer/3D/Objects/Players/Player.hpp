/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Player.hpp
*/

#pragma once

#include "IPlayer.hpp"
#include "../IMesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <string>
#include <memory>

namespace renderer3d
{

    class Player : public IPlayer
    {
    public:
        Player(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl, const std::string &team);
        ~Player() override;

        void draw() override;
        void update(float deltaTime) override;

        void setPosition(const glm::vec3 &pos) override;
        glm::vec3 getPosition() const override;

        void setRotation(const glm::vec3 &rot) override;
        glm::vec3 getRotation() const override;

        void setScale(const glm::vec3 &scl) { scale = scl; }
        glm::vec3 getScale() const { return scale; }

        void setTeam(const std::string &team) override;
        std::string getTeam() const override;

        void setAuraColor(const glm::vec3 &color) override;
        glm::vec3 getAuraColor() const override;

        void move(const glm::vec3 &direction, float speed);
        void rotate(const glm::vec3 &axis, float angle);

        void setMesh(IMesh *m) { mesh = m; }
        IMesh *getMesh() const { return mesh; }

        void setTextureId(GLuint id) { textureId = id; }
        GLuint getTextureId() const { return textureId; }

        void setId(int id) override {
            playerId = id;
        }

        int getId() const override {
            return playerId;
        }

    private:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        std::string team;
        glm::vec3 auraColor;

        IMesh *mesh;
        GLuint textureId;
        int playerId;  // Add this field to store the player ID
    };
}
