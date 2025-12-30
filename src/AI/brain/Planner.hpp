/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Planner.hpp - AI decision making and strategy coordination
*/

#pragma once
#include "Strategy.hpp"
#include "SurviveStrategy.hpp"
#include "ElevationStrategy.hpp"
#include "MapMemory.hpp"
#include <memory>
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <queue>

/**
 * @brief Main AI planner that coordinates strategies and decision making
 * 
 * The planner manages multiple strategies and decides which one to use
 * based on the current world state. It also handles:
 * - Command failure recovery
 * - Resource mapping and tracking
 * - BFS pathfinding for navigation
 * - Strategy coordination and fallbacks
 */
class Planner
{
public:
    /**
     * @brief Constructs a new planner with default strategies
     */
    Planner();

    /**
     * @brief Sets the world dimensions for navigation calculations
     * 
     * @param width World width
     * @param height World height
     */
    void setWorldSize(int width, int height);

    /**
     * @brief Sets the result of the last command
     * 
     * @param result Command result ("ok", "ko", "Elevation underway", etc.)
     */
    void setLastResult(const std::string &result);

    /**
     * @brief Notifies that an incantation command was sent
     */
    void setIncantationSent();

    /**
     * @brief Handles broadcast messages from teammates
     * 
     * @param message Broadcast message content
     */
    void handleBroadcastMessage(const std::string &message);

    /**
     * @brief Gets access to the strategies vector
     * 
     * @return Reference to the strategies vector
     */
    const std::vector<std::unique_ptr<Strategy>>& getStrategies() const;

    /**
     * @brief Main decision making method
     * 
     * Evaluates all strategies and returns the best commands to execute.
     * Handles command failures and strategy coordination.
     * 
     * @param st Current world state
     * @return Vector of commands to execute
     */
    std::vector<std::string> decide(const WorldState &st);

private:
    /**
     * @brief Handles command failures and recovery
     * 
     * @param st Current world state
     * @param prevCommand The command that failed
     * @return Recovery commands
     */
    std::vector<std::string> handleCommandFailure(const WorldState &st, const std::string &prevCommand);

    /**
     * @brief Updates the local resource map based on current vision
     * 
     * @param st Current world state
     */
    void updateLocalResourceMap(const WorldState &st);

    /**
     * @brief Gets the position of a tile based on vision index
     * 
     * @param st Current world state
     * @param visionIndex Index in vision array
     * @return Tile position
     */
    std::pair<int, int> getTilePosition(const WorldState &st, size_t visionIndex);

    /**
     * @brief Removes a resource from the local map
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @param resource Resource to remove
     */
    void removeResourceFromLocalMap(int x, int y, const std::string &resource);

    /**
     * @brief Checks if there's food on the current tile
     * 
     * @param st Current world state
     * @return true if food is present, false otherwise
     */
    bool hasFoodOnCurrentTile(const WorldState &st);

    /**
     * @brief Finds a resource in vision
     * 
     * @param st Current world state
     * @param resource Resource to find
     * @return Vision index where resource is found, or -1 if not found
     */
    int findResourceInVision(const WorldState &st, const std::string &resource);

    /**
     * @brief Handles low food situation
     * 
     * @param st Current world state
     * @return Commands to collect food
     */
    std::vector<std::string> handleLowFood(const WorldState &st);

    /**
     * @brief Finds a resource in the local map
     * 
     * @param resource Resource to find
     * @return Vector of positions where resource is found
     */
    std::vector<std::pair<int, int>> findResourceInLocalMap(const std::string &resource);

    /**
     * @brief Gets unvisited tiles
     * 
     * @return Vector of unvisited tile positions
     */
    std::vector<std::pair<int, int>> getUnvisitedTiles();

    /**
     * @brief Finds BFS path to target
     * 
     * @param st Current world state
     * @param target Target position
     * @return Path to target
     */
    std::vector<std::pair<int, int>> bfsPathToTarget(const WorldState &st, const std::pair<int, int> &target);

    /**
     * @brief Finds the closest target from a list
     * 
     * @param st Current world state
     * @param targets List of target positions
     * @return Closest target position
     */
    std::pair<int, int> findClosestTarget(const WorldState &st, const std::vector<std::pair<int, int>> &targets);

    /**
     * @brief Navigates to a target with organic movement patterns
     * 
     * @param st Current world state
     * @param targets List of target positions
     * @return Navigation command to execute
     */
    std::string navigateToTarget(const WorldState &st, const std::vector<std::pair<int, int>> &targets = {});

    std::vector<std::unique_ptr<Strategy>> strategies;
    std::string lastCommand;
    std::string lastResult;
    int moveFailureCount;
    int sameCommandCount;
    int lookCounter;
    int spiralStep;
    int worldWidth;
    int worldHeight;
    std::unordered_set<std::pair<int, int>, pair_hash> visitedTiles;
    std::unordered_map<std::pair<int, int>, std::vector<std::string>, pair_hash> resourceMap;
}; 