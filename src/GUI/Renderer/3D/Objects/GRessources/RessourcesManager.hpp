/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RessourcesManager.hpp
*/

#pragma once

#include "IRessources.hpp"
#include "RessourcesFactory.hpp"
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class RessourcesManager
{
public:
    RessourcesManager() = default;
    ~RessourcesManager() = default;

    IRessources *addRessource(const std::string &type, const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scl)
    {
        auto res = RessourcesFactory::create(type, pos, rot, scl);
        if (res)
        {
            ressources.push_back(std::move(res));
            return ressources.back().get();
        }
        return nullptr;
    }

    void updateAll(float deltaTime)
    {
        for (auto &res : ressources)
            res->update(deltaTime);
    }

    void drawAll()
    {
        for (auto &res : ressources)
            res->draw();
    }

    void clear()
    {
        ressources.clear();
    }

    std::size_t getRessourceCount() const
    {
        return ressources.size();
    }

private:
    std::vector<std::unique_ptr<IRessources>> ressources;
};
