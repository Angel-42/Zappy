/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ElevationStrategy.hpp - Player elevation (incantation) logic
*/

#pragma once
#include "Strategy.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include "SurviveStrategy.hpp"

/**
 * @brief Requirements for player elevation at each level
 * 
 * Defines the exact number of players and resources needed
 * to successfully complete an elevation ritual.
 */
struct ElevationRequirements
{
    int players;    ///< Number of players required
    int linemate;   ///< Linemate resources required
    int deraumere;  ///< Deraumere resources required
    int sibur;      ///< Sibur resources required
    int mendiane;   ///< Mendiane resources required
    int phiras;     ///< Phiras resources required
    int thystame;   ///< Thystame resources required
};

/**
 * @brief Strategy for handling player elevation (incantation)
 * 
 * This strategy manages the elevation process by:
 * - Checking if the player can elevate based on resources and requirements
 * - Coordinating with teammates for multi-player elevations
 * - Collecting missing resources needed for elevation
 * - Managing the incantation state and cooldowns
 * 
 * The strategy follows the Zappy protocol where elevation requires:
 * - Specific resources based on current level
 * - Multiple players on the same tile (for higher levels)
 * - A 300-second cooldown between incantation attempts
 */
class ElevationStrategy : public Strategy
{
public:
    /**
     * @brief Constructs a new elevation strategy
     */
    ElevationStrategy();

    /**
     * @brief Determines if this strategy should be applied
     * 
     * @param st Current world state
     * @return true if elevation should be attempted, false otherwise
     */
    bool applicable(const WorldState &st) const override;

    /**
     * @brief Generates commands for elevation or resource collection
     * 
     * @param st Current world state
     * @return Vector of commands to execute
     */
    std::vector<std::string> getCommands(const WorldState &st) override;

    /**
     * @brief Handles the result of an incantation attempt
     * 
     * @param result Server response ("Elevation underway" or "ko")
     */
    void setIncantationResult(const std::string &result);

    /**
     * @brief Called when an incantation command is sent to the server
     */
    void setIncantationSent();

    /**
     * @brief Processes broadcast messages from teammates
     * 
     * @param message Broadcast message content
     */
    void setBroadcastMessage(const std::string &message);

    /**
     * @brief Checks if the AI is currently in incantation mode
     * 
     * @return true if incantation is active, false otherwise
     */
    bool getIsInIncantation() const;

private:
    static const std::unordered_map<int, ElevationRequirements> elevationRules;

    bool isInIncantation = false;
    bool incantationQueued = false;
    int lastElevationTime = 0;
    int elevationFailureCount = 0;
    int lastBroadcastTime = 0;
    int broadcastCooldown = 10;
    bool teammateNeedsElevation = false;
    bool teammateReadyForElevation = false;

    /**
     * @brief Checks if the player can elevate based on current resources
     * 
     * @param st Current world state
     * @return true if elevation is possible, false otherwise
     */
    bool canElevate(const WorldState &st) const;

    /**
     * @brief Checks if the player has all required resources for elevation
     * 
     * @param st Current world state
     * @return true if all resources are available, false otherwise
     */
    bool hasAllResourcesForElevation(const WorldState &st) const;

    /**
     * @brief Gets the list of missing resources for elevation
     * 
     * @param st Current world state
     * @return Vector of missing resource names
     */
    std::vector<std::string> getMissingResources(const WorldState &st) const;

    /**
     * @brief Generates commands to collect missing resources
     * 
     * @param st Current world state
     * @param missingResources List of resources to collect
     * @return Vector of commands to collect resources
     */
    std::vector<std::string> collectMissingResources(const WorldState &st, const std::vector<std::string> &missingResources);

    /**
     * @brief Coordinates with teammates for multi-player elevation
     * 
     * @param st Current world state
     * @return Vector of coordination commands
     */
    std::vector<std::string> coordinateWithTeam(const WorldState &st);

    /**
     * @brief Explores the map to find resources needed for elevation
     * 
     * @param st Current world state
     * @return Vector of exploration commands
     */
    std::vector<std::string> exploreForResources(const WorldState &st);

    /**
     * @brief Gets elevation requirements for a specific level
     * 
     * @param level Player level
     * @return Pointer to elevation requirements, or nullptr if level not found
     */
    const ElevationRequirements* getElevationRequirements(int level) const;
}; 