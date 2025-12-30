#pragma once
#include <string>
#include <vector>
#include "ResourceType.hpp"

namespace clientapi {

/**
 * @brief Represents a team in the game.
 * 
 * Stores team name, player IDs, and egg IDs.
 */
class Team {
public:
    /**
     * @brief Default constructor
     */
    Team() = default;
    /**
     * @brief Construct a new Team object
     * @param name The team's name
     */
    Team(const std::string& name);
    /**
     * @brief Get the team's name
     * @return The team name
     */
    const std::string& getName() const;
    /**
     * @brief Get the list of player IDs in the team
     * @return Vector of player IDs
     */
    const std::vector<int>& getPlayers() const;
    /**
     * @brief Get the list of egg IDs belonging to the team
     * @return Vector of egg IDs
     */
    const std::vector<int>& getEggs() const;

    /**
     * @brief Add a player to the team
     * @param playerId The player's unique ID
     */
    void addPlayer(int playerId);
    /**
     * @brief Remove a player from the team
     * @param playerId The player's unique ID
     */
    void removePlayer(int playerId);
    /**
     * @brief Add an egg to the team
     * @param eggId The egg's unique ID
     */
    void addEgg(int eggId);
    /**
     * @brief Remove an egg from the team
     * @param eggId The egg's unique ID
     */
    void removeEgg(int eggId);

private:
    std::string _name;
    std::vector<int> _players;
    std::vector<int> _eggs;
};

} // namespace clientapi 