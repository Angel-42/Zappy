/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** SurviveStrategy.hpp - Basic survival and resource collection
*/

#pragma once
#include "Strategy.hpp"
#include <algorithm>
#include <iostream>
#include <functional>

/**
 * @brief Strategy for basic survival and resource collection
 * 
 * This strategy handles the fundamental survival needs of the AI:
 * - Collecting food to maintain health
 * - Gathering minerals for elevation
 * - Exploring the map when resources are not visible
 * - Using BFS pathfinding to navigate to known resource locations
 * 
 * The strategy prioritizes food when health is low and switches
 * to mineral collection when food is sufficient.
 */
class SurviveStrategy : public Strategy
{
public:
    /**
     * @brief Direction enumeration for vision-based navigation
     */
    enum class Direction
    {
        HERE,           ///< Current tile
        FORWARD,        ///< One step forward
        FORWARD_LEFT,   ///< Forward and left
        FORWARD_RIGHT,  ///< Forward and right
        LEFT,           ///< One step left
        RIGHT,          ///< One step right
        BACK,           ///< One step back
        UNKNOWN         ///< Unknown direction
    };

    /** @brief Static function pointer for BFS navigation */
    static std::function<std::string(const WorldState&, const std::vector<std::pair<int, int>>&)> bfsNavigate;

    /**
     * @brief Converts vision index to direction
     * 
     * @param idx Vision index (0-6)
     * @return Corresponding direction
     */
    static Direction visionIndexToDirection(size_t idx);

    /**
     * @brief Always applicable as fallback strategy
     */
    bool applicable(const WorldState &) const override;

    /**
     * @brief Generates commands for survival and resource collection
     * 
     * @param st Current world state
     * @return Vector of commands to execute
     */
    std::vector<std::string> getCommands(const WorldState &st) override;

private:
    /**
     * @brief Handles resource collection when food is low
     * 
     * @param st Current world state
     * @return Vector of commands for food collection
     */
    std::vector<std::string> handleLowFood(const WorldState &st);

    /**
     * @brief Handles mineral collection when food is sufficient
     * 
     * @param st Current world state
     * @return Vector of commands for mineral collection
     */
    std::vector<std::string> handleMineralCollection(const WorldState &st);

    /**
     * @brief Handles mineral collection when food is sufficient
     * 
     * @param st Current world state
     * @param ignoreFood Ignore food condition
     * @return Vector of commands for mineral collection
     */
    std::vector<std::string> handleMineralCollection(const WorldState &st, bool ignoreFood);

    /**
     * @brief Collects a specific mineral from current tile or navigates to it
     * 
     * @param st Current world state
     * @param mineral Mineral to collect
     * @return Vector of commands for mineral collection
     */
    std::vector<std::string> collectMineral(const WorldState &st, const std::string &mineral);

    /**
     * @brief Fallback exploration when no specific resources are targeted
     * 
     * @param st Current world state
     * @return Vector of exploration commands
     */
    std::vector<std::string> fallbackExploration(const WorldState &st);

    /**
     * @brief Checks if an item is a mineral
     * 
     * @param item Item to check
     * @return true if item is a mineral, false otherwise
     */
    static bool isMineral(const std::string &item);
}; 