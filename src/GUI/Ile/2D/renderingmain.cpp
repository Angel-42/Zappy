/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** renderingmain
*/

#include "Rendering.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 4 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <map_size> <player_count> <food_count>" << std::endl;
        return 1;
    }

    Rendering rendering;
    int mapSize = std::stoi(argv[1]);
    int playerCount = std::stoi(argv[2]);
    int foodCount = std::stoi(argv[3]);

    if (mapSize <= 0 || playerCount < 0 || mapSize < playerCount) {
        std::cerr << "Invalid map size or player count." << std::endl;
        return 1;
    }

    rendering.setMapSize(mapSize);
    rendering.setPlayerCount(playerCount);
    rendering.setFoodCount(foodCount);
    rendering.initiateDrawables();
    while (rendering.isOpen()) {
        rendering.update();
    }
    return 0;
}