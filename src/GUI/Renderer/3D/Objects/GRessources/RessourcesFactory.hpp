/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RessourcesFactory.hpp
*/

#pragma once

#include "Minerals/IMinerals.hpp"
#include "Minerals/Thystame/Thystame.hpp"
#include "Minerals/Deraumere/Deraumere.hpp"
#include "Minerals/Phiras/Phiras.hpp"
#include "Minerals/Linemate/Linemate.hpp"
#include "Minerals/Sibur/Sibur.hpp"
#include "Minerals/Mendiane/Mendiane.hpp"
#include "Food/Food.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <string>

class RessourcesFactory
{
public:
    static std::unique_ptr<IRessources> create(
        const std::string &type,
        const glm::vec3 &pos,
        const glm::vec3 &rot,
        const glm::vec3 &scl)
    {
        if (type == "Thystame")
            return std::make_unique<Thystame>(pos, rot, scl);
        else if (type == "Deraumere")
            return std::make_unique<Deraumere>(pos, rot, scl);
        else if (type == "Phiras")
            return std::make_unique<Phiras>(pos, rot, scl);
        else if (type == "Linemate")
            return std::make_unique<Linemate>(pos, rot, scl);
        else if (type == "Sibur")
            return std::make_unique<Sibur>(pos, rot, scl);
        else if (type == "Mendiane")
            return std::make_unique<Mendiane>(pos, rot, scl);
        else if (type == "Food")
            return std::make_unique<Food>(pos, rot, scl);
        else
            return nullptr;

        return nullptr; // Unknown type
    }
};
