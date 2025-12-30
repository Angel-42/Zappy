/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** WorldState.hpp
*/

#pragma once
#include "brain/MapMemory.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

enum class Orientation
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

struct WorldState
{
    MapMemory mapMemory;
    WorldState(int width = 0, int height = 0)
        : mapMemory() {}

    int level = 1;
    int food = 10;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;

    std::unordered_map<std::string, int> inventory;
    std::vector<std::vector<std::string>> vision;

    int posX = 0;
    int posY = 0;
    Orientation orientation = Orientation::NORTH;
};

class WorldUtil
{
public:
    static int getInventoryCount(const WorldState &state, const std::string &item)
    {
        auto it = state.inventory.find(item);
        return it != state.inventory.end() ? it->second : 0;
    }

    static bool hasItem(const WorldState &state, const std::string &item)
    {
        return getInventoryCount(state, item) > 0;
    }

    static bool isVisionEmpty(const WorldState &state)
    {
        return state.vision.empty();
    }

    static int getFoodCount(const WorldState &state)
    {
        return state.food;
    }

    static int getLevel(const WorldState &state)
    {
        return state.level;
    }

    static std::vector<std::pair<int, int>> findResourceTiles(const WorldState &state, const std::string &resource)
    {
        return state.mapMemory.findResource(resource);
    }

    static bool isResourceNearby(const WorldState &state, const std::string &resource, int px, int py, int radius = 1)
    {
        for (int dy = -radius; dy <= radius; ++dy)
            for (int dx = -radius; dx <= radius; ++dx)
            {
                int nx = px + dx, ny = py + dy;
                const auto &tile = state.mapMemory.getTile(nx, ny);
                if (!tile.empty() && std::find(tile.begin(), tile.end(), resource) != tile.end())
                    return true;
            }
        return false;
    }

    static std::vector<EggInfo> getAllEggs(const WorldState &state)
    {
        std::vector<EggInfo> eggs;
        for (const auto &pair : state.mapMemory.eggs)
            eggs.push_back(pair.second);
        return eggs;
    }
};
