/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IPlayer.hpp
*/

#pragma once

#include <string>
#include <glm/glm.hpp>

class IPlayer
{
public:
    virtual ~IPlayer() = default;

    virtual void draw() = 0;
    virtual void update(float deltaTime) = 0;

    virtual void setPosition(const glm::vec3 &pos) = 0;
    virtual glm::vec3 getPosition() const = 0;

    virtual void setRotation(const glm::vec3 &rot) = 0;
    virtual glm::vec3 getRotation() const = 0;
    
    virtual void setScale(const glm::vec3 &scale) = 0;
    virtual glm::vec3 getScale() const = 0;

    virtual void setTeam(const std::string &team) = 0;
    virtual std::string getTeam() const = 0;

    virtual void setAuraColor(const glm::vec3 &color) = 0;
    virtual glm::vec3 getAuraColor() const = 0;
    
    virtual void setId(int id) = 0;
    virtual int getId() const = 0;
};
