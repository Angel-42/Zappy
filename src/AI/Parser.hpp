/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Parser.hpp
*/

#pragma once
#include "WorldState.hpp"
#include <string>

class Parser
{
public:
    explicit Parser(WorldState &s);

    void ingest(const std::string &line);
    void onCommandAck(const std::string &cmd);
    void setWorldSize(int width, int height);

private:
    int worldWidth = 0;
    int worldHeight = 0;

    std::pair<int, int> normalizePosition(int x, int y) const;
    void parseInventory(const std::string &payload);
    void parseLook(const std::string &payload);
    std::pair<int, int> calculateTileOffset(int tileIndex, Orientation orientation) const;

    WorldState &st;
};
