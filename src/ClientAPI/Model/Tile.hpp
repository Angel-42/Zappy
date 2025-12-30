#pragma once
#include <vector>
#include <array>
#include "ResourceType.hpp"
#include "Player.hpp"
#include "Egg.hpp"

namespace clientapi {

/**
 * @brief Represents a single cell on the map, containing resources, players, and eggs.
 */
class Tile {
public:
    /**
     * @brief Construct a new Tile object
     * @param x The x coordinate
     * @param y The y coordinate
     */
    Tile(int x, int y);
    /**
     * @brief Get the x coordinate of the tile
     * @return The x coordinate
     */
    int getX() const;
    /**
     * @brief Get the y coordinate of the tile
     * @return The y coordinate
     */
    int getY() const;
    /**
     * @brief Get the resources present on the tile
     * @return Array of resource counts
     */
    const std::array<int, ResourceType::COUNT>& getResources() const;
    /**
     * @brief Get the list of player IDs present on the tile
     * @return Vector of player IDs
     */
    const std::vector<int>& getPlayers() const;
    /**
     * @brief Get the list of egg IDs present on the tile
     * @return Vector of egg IDs
     */
    const std::vector<int>& getEggs() const;

    /**
     * @brief Set the amount of a specific resource on the tile
     * @param type The resource type
     * @param amount The new amount
     */
    void setResource(ResourceType type, int amount);
    /**
     * @brief Set all resources on the tile
     * @param resources Array of resource counts
     */
    void setResources(const std::array<int, ResourceType::COUNT>& resources);
    /**
     * @brief Add a player to the tile
     * @param playerId The player's unique ID
     */
    void addPlayer(int playerId);
    /**
     * @brief Remove a player from the tile
     * @param playerId The player's unique ID
     */
    void removePlayer(int playerId);
    /**
     * @brief Add an egg to the tile
     * @param eggId The egg's unique ID
     */
    void addEgg(int eggId);
    /**
     * @brief Remove an egg from the tile
     * @param eggId The egg's unique ID
     */
    void removeEgg(int eggId);

private:
    int _x, _y;
    std::array<int, ResourceType::COUNT> _resources{};
    std::vector<int> _players;
    std::vector<int> _eggs;
};

} // namespace clientapi 