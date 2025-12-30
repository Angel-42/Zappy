/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Player.cpp
*/

#include "Player.hpp"

namespace renderer3d
{

    Player::Player(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &sc, const std::string &t)
        : position(pos), rotation(rot), scale(sc), team(t), mesh(nullptr), playerId(-1) // Initialize playerId
    {
        auraColor = glm::vec3(1.0f, 0.0f, 0.0f);
    }

    Player::~Player() {}

    void Player::draw()
    {
        if (mesh)
        {
            mesh->draw();
        }
    }

    void Player::update(float deltaTime)
    {
    }

    void Player::setPosition(const glm::vec3 &pos) { position = pos; }
    glm::vec3 Player::getPosition() const { return position; }

    void Player::setRotation(const glm::vec3 &rot) { rotation = rot; }
    glm::vec3 Player::getRotation() const { return rotation; }

    void Player::setTeam(const std::string &t) { team = t; }
    std::string Player::getTeam() const { return team; }

    void Player::setAuraColor(const glm::vec3 &color) { auraColor = color; }
    glm::vec3 Player::getAuraColor() const { return auraColor; }

    void Player::move(const glm::vec3 &direction, float speed)
    {
        position += direction * speed;
    }

    void Player::rotate(const glm::vec3 &axis, float angle)
    {
        rotation = glm::rotate(rotation, glm::radians(angle), axis);
    }
}
