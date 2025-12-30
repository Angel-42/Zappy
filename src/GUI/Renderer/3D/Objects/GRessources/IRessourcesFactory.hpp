/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RessourcesFactory.hpp
*/

#pragma once

#include "IRessources.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <string>

class IRessourcesFactory
{
public:
    virtual ~IRessourcesFactory() = default;
    virtual std::unique_ptr<IRessources> create(
        const std::string &type,
        const glm::vec3 &pos,
        const glm::vec3 &rot,
        const glm::vec3 &scl) = 0;
};
