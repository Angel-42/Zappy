/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameManager - Manages game state and coordinates all game systems
*/

#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <string>
#include <vector>
#include <memory>
#include "Model/Player.hpp"
#include "Model/Map.hpp"
#include "Model/Team.hpp"
#include "Model/ResourceType.hpp"
#include "Model/ElevationRules.hpp"
#include "Model/GameState.hpp"

namespace clientapi {

// Forward declarations to avoid circular dependencies
class Map;
class PlayerManager; 
class TeamManager;
class Team;
class Player;
class ClientHandler;

class GameManager {
    public:
        /**
         * @brief Get the singleton instance of GameManager
         * @return Reference to the singleton instance
         */
        static GameManager& getInstance();

        /**
         * @brief Construct a new GameManager object
         */
        GameManager();

        /**
         * @brief Destroy the GameManager object
         */
        ~GameManager();

        void startGame();
        void pauseGame();
        void endGame();
        clientapi::State getGameState() const { return _gameState; }

        void checkGameEnd(Map* map, PlayerManager* playerManager, TeamManager* teamManager);
        bool processIncantation(Player* player, Map* map, PlayerManager* playerManager);
        void processPlayerAction(Player* player, const std::string& action, Map* map);
        
        Team* getWinningTeam() const;
        bool isGameComplete() const;
        
        bool canPlayerPerformAction(Player* player, const std::string& action) const;
        void handlePlayerDeath(Player* player, Map* map, PlayerManager* playerManager);
        void respawnPlayer(Player* player, Map* map);

        /**
         * @brief Get the current map dimensions
         * @return Pair of (width, height)
         */
        std::pair<int, int> getMapSize() const;

        /**
         * @brief Get the content of a specific tile
         * @param x X coordinate
         * @param y Y coordinate
         * @return Vector of resource quantities [food, linemate, deraumere, sibur, mendiane, phiras, thystame]
         */
        std::vector<int> getTileContent(int x, int y) const;

        /**
         * @brief Get all tile contents for the entire map
         * @return Vector of tile contents for all positions
         */
        std::vector<std::vector<int>> getAllTileContents() const;

        /**
         * @brief Get the list of team names
         * @return Vector of team names
         */
        std::vector<std::string> getTeamNames() const;

        /**
         * @brief Get the current time unit
         * @return Current time unit value
         */
        int getTimeUnit() const;

        /**
         * @brief Set the time unit
         * @param timeUnit New time unit value
         */
        void setTimeUnit(int timeUnit);

        /**
         * @brief Get all players in the game
         * @return Vector of player pointers
         */
        std::vector<Player*> getAllPlayers() const;

        /**
         * @brief Get a specific player by ID
         * @param playerId The player ID
         * @return Pointer to player, or nullptr if not found
         */
        Player* getPlayer(int playerId) const;

        /**
         * @brief Broadcast a game event to all GUI clients
         * @param event The event message to broadcast
         */
        void broadcastToGUI(const std::string& event);

        /**
         * @brief Register a GUI client for receiving game updates
         * @param clientHandler Pointer to the GUI client handler
         */
        void registerGUIClient(ClientHandler* clientHandler);

        /**
         * @brief Unregister a GUI client
         * @param clientHandler Pointer to the GUI client handler
         */
        void unregisterGUIClient(ClientHandler* clientHandler);

        // Event broadcasting methods for game events
        /**
         * @brief Broadcast new player event to all GUI clients
         * @param player The new player
         */
        void broadcastPlayerNew(Player* player);

        /**
         * @brief Broadcast player position update to all GUI clients
         * @param player The player whose position changed
         */
        void broadcastPlayerPosition(Player* player);

        /**
         * @brief Broadcast player level update to all GUI clients
         * @param player The player whose level changed
         */
        void broadcastPlayerLevel(Player* player);

        /**
         * @brief Broadcast player inventory update to all GUI clients
         * @param player The player whose inventory changed
         */
        void broadcastPlayerInventory(Player* player);

        /**
         * @brief Broadcast player death event to all GUI clients
         * @param player The player who died
         */
        void broadcastPlayerDeath(Player* player);

        /**
         * @brief Broadcast player expulsion event to all GUI clients
         * @param player The player who was expelled
         */
        void broadcastPlayerExpulsion(Player* player);

        /**
         * @brief Broadcast player broadcast message to all GUI clients
         * @param player The player who sent the broadcast
         * @param message The broadcast message
         */
        void broadcastPlayerBroadcast(Player* player, const std::string& message);

        /**
         * @brief Broadcast incantation start event to all GUI clients
         * @param x Incantation X coordinate
         * @param y Incantation Y coordinate
         * @param level Incantation level
         * @param players Vector of participating player IDs
         */
        void broadcastIncantationStart(int x, int y, int level, const std::vector<int>& players);

        /**
         * @brief Broadcast incantation end event to all GUI clients
         * @param x Incantation X coordinate
         * @param y Incantation Y coordinate
         * @param result Incantation result (0 for failure, 1 for success)
         */
        void broadcastIncantationEnd(int x, int y, int result);

        /**
         * @brief Broadcast resource drop event to all GUI clients
         * @param player The player who dropped the resource
         * @param resourceId The resource ID that was dropped
         */
        void broadcastResourceDrop(Player* player, int resourceId);

        /**
         * @brief Broadcast resource collect event to all GUI clients
         * @param player The player who collected the resource
         * @param resourceId The resource ID that was collected
         */
        void broadcastResourceCollect(Player* player, int resourceId);

        /**
         * @brief Broadcast egg laid event to all GUI clients
         * @param eggId The egg ID
         * @param player The player who laid the egg
         * @param x Egg X coordinate
         * @param y Egg Y coordinate
         */
        void broadcastEggLaid(int eggId, Player* player, int x, int y);

        /**
         * @brief Broadcast egg hatching event to all GUI clients
         * @param eggId The egg ID
         */
        void broadcastEggHatching(int eggId);

        /**
         * @brief Broadcast egg death event to all GUI clients
         * @param eggId The egg ID
         */
        void broadcastEggDeath(int eggId);

        /**
         * @brief Broadcast game end event to all GUI clients
         * @param winningTeam Name of the winning team
         */
        void broadcastGameEnd(const std::string& winningTeam);

        /**
         * @brief Broadcast server message to all GUI clients
         * @param message The server message
         */
        void broadcastServerMessage(const std::string& message);

    private:
        clientapi::State _gameState;
        std::unique_ptr<ElevationRules> _elevationRules;
        Team* _winningTeam;
        Map* _map;
        std::vector<Team> _teams;
        int _timeUnit;
        std::vector<ClientHandler*> _guiClients;  ///< List of connected GUI clients for broadcasting

        void checkWinConditions(TeamManager* teamManager);
        void initializeGameRules();
        
        /**
         * @brief Send complete initial game state to a new GUI client
         * @param guiClient The GUI client to send the state to
         */
        void sendInitialGameState(ClientHandler* guiClient);
};

} // namespace clientapi

#endif /* !GAMEMANAGER_HPP_ */ 