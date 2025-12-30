#pragma once
#include <unordered_map>
#include <memory>
#include "Player.hpp"
#include "Team.hpp"
#include "Egg.hpp"
#include "Map.hpp"

namespace clientapi {

enum State {
    WAITING,
    RUNNING,
    ENDED
};

/**
 * @brief Represents the current state of the game, including all players, teams, eggs, and the map.
 * 
 * GameState acts as the central container for all game-related entities and provides methods to add, update, remove, and retrieve them.
 */
class GameState {
public:
    /**
     * @brief Construct a new GameState object
     */
    GameState();
    /**
     * @brief Get a pointer to a player by ID
     * @param id The player's unique ID
     * @return Pointer to the Player, or nullptr if not found
     */
    Player* getPlayer(int id);
    /**
     * @brief Get a const pointer to a player by ID
     * @param id The player's unique ID
     * @return Const pointer to the Player, or nullptr if not found
     */
    const Player* getPlayer(int id) const;
    /**
     * @brief Add or update a player in the game state
     * @param player The Player object to add or update
     */
    void addOrUpdatePlayer(const Player& player);
    /**
     * @brief Remove a player from the game state
     * @param id The player's unique ID
     */
    void removePlayer(int id);
    /**
     * @brief Get a pointer to a team by name
     * @param name The team's name
     * @return Pointer to the Team, or nullptr if not found
     */
    Team* getTeam(const std::string& name);
    /**
     * @brief Get a const pointer to a team by name
     * @param name The team's name
     * @return Const pointer to the Team, or nullptr if not found
     */
    const Team* getTeam(const std::string& name) const;
    /**
     * @brief Add or update a team in the game state
     * @param team The Team object to add or update
     */
    void addOrUpdateTeam(const Team& team);
    /**
     * @brief Remove a team from the game state
     * @param name The team's name
     */
    void removeTeam(const std::string& name);
    /**
     * @brief Get a pointer to an egg by ID
     * @param id The egg's unique ID
     * @return Pointer to the Egg, or nullptr if not found
     */
    Egg* getEgg(int id);
    /**
     * @brief Get a const pointer to an egg by ID
     * @param id The egg's unique ID
     * @return Const pointer to the Egg, or nullptr if not found
     */
    const Egg* getEgg(int id) const;
    /**
     * @brief Add or update an egg in the game state
     * @param egg The Egg object to add or update
     */
    void addOrUpdateEgg(const Egg& egg);
    /**
     * @brief Remove an egg from the game state
     * @param id The egg's unique ID
     */
    void removeEgg(int id);
    /**
     * @brief Get a pointer to the map
     * @return Pointer to the Map, or nullptr if not set
     */
    Map* getMap();
    /**
     * @brief Get a const pointer to the map
     * @return Const pointer to the Map, or nullptr if not set
     */
    const Map* getMap() const;
    /**
     * @brief Set the map for the game state
     * @param map The Map object to set
     */
    void setMap(const Map& map);
    /**
     * @brief Get all players in the game state
     * @return Const reference to the map of player IDs to Player objects
     */
    const std::unordered_map<int, Player>& getPlayers() const;
    /**
     * @brief Get all teams in the game state
     * @return Const reference to the map of team names to Team objects
     */
    const std::unordered_map<std::string, Team>& getTeams() const;
    /**
     * @brief Get all eggs in the game state
     * @return Const reference to the map of egg IDs to Egg objects
     */
    const std::unordered_map<int, Egg>& getEggs() const;

private:
    std::unordered_map<int, Player> _players;
    std::unordered_map<std::string, Team> _teams;
    std::unordered_map<int, Egg> _eggs;
    std::unique_ptr<Map> _map;
};

} // namespace clientapi 