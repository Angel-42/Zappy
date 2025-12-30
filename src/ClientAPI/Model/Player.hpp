#pragma once
#include <string>
#include <vector>
#include <array>
#include "ResourceType.hpp"

namespace clientapi {

/**
 * @brief Represents a position on the map.
 */
struct Position {
    int x = 0; ///< X coordinate
    int y = 0; ///< Y coordinate
    /**
     * @brief Default constructor
     */
    Position() = default;
    /**
     * @brief Construct a position with coordinates
     * @param x_ X coordinate
     * @param y_ Y coordinate
     */
    Position(int x_, int y_) : x(x_), y(y_) {}
};

/**
 * @brief Represents a player in the game.
 * 
 * Stores player ID, team, position, orientation, level, inventory, and socket.
 */
class Player {
public:
    /**
     * @brief Default constructor
     */
    Player() = default;
    /**
     * @brief Construct a new Player object
     * @param id The player's unique ID
     * @param team The player's team name
     * @param pos The player's position
     */
    Player(int id, const std::string& team, const Position& pos);
    /**
     * @brief Get the player's unique ID
     * @return The player ID
     */
    int getId() const;
    /**
     * @brief Get the player's team name
     * @return The team name
     */
    const std::string& getTeam() const;
    /**
     * @brief Get the player's position
     * @return The position struct
     */
    const Position& getPosition() const;
    /**
     * @brief Get the player's orientation (1=N, 2=E, 3=S, 4=W)
     * @return The orientation value
     */
    int getOrientation() const;
    /**
     * @brief Get the player's level
     * @return The level
     */
    int getLevel() const;
    /**
     * @brief Get the player's inventory (resource counts)
     * @return Array of resource counts
     */
    const std::array<int, ResourceType::COUNT>& getInventory() const;
    /**
     * @brief Get the player's team name (alias)
     * @return The team name
     */
    const std::string& getTeamName() const { return _team; }
    /**
     * @brief Get the amount of food in the player's inventory
     * @return The food count
     */
    int getFood() const { return _inventory[ResourceType::FOOD]; }
    /**
     * @brief Get the count of a specific resource in the player's inventory
     * @param type The resource type
     * @return The resource count
     */
    int getResourceCount(ResourceType type) const { return _inventory[type]; }
    /**
     * @brief Set the player's position
     * @param pos The new position
     */
    void setPosition(const Position& pos);
    /**
     * @brief Set the player's orientation
     * @param orientation The new orientation
     */
    void setOrientation(int orientation);
    /**
     * @brief Set the player's level
     * @param level The new level
     */
    void setLevel(int level);
    /**
     * @brief Set the player's inventory
     * @param inv The new inventory array
     */
    void setInventory(const std::array<int, ResourceType::COUNT>& inv);
    /**
     * @brief Set the amount of a specific resource in the player's inventory
     * @param type The resource type
     * @param amount The new amount
     */
    void setResource(ResourceType type, int amount);
    /**
     * @brief Turn the player right (clockwise)
     */
    void turnRight() { _orientation = _orientation % 4 + 1; }
    /**
     * @brief Turn the player left (counterclockwise)
     */
    void turnLeft() { _orientation = (_orientation + 2) % 4 + 1; }
    /**
     * @brief Set the player's socket descriptor
     * @param socket The socket descriptor
     */
    void setSocket(int socket) { _socket = socket; }
    /**
     * @brief Get the player's socket descriptor
     * @return The socket descriptor
     */
    int getSocket() const { return _socket; }

private:
    int _id;
    std::string _team;
    Position _position;
    int _orientation = 1; // 1=N, 2=E, 3=S, 4=W
    int _level = 1;
    std::array<int, ResourceType::COUNT> _inventory{};
    int _socket = -1;
};

} // namespace clientapi 