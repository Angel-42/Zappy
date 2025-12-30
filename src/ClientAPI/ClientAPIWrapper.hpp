/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientAPIWrapper - Main game client API wrapper controller
*/

#ifndef CLIENTAPIWRAPPER_HPP_
#define CLIENTAPIWRAPPER_HPP_

#include <memory>
#include <functional>
#include <string>
#include <vector>
#include <array>
#include "Network/NetworkManager.hpp"
#include "Network/Protocol.hpp"
#include "Model/GameState.hpp"
#include "Model/ResourceType.hpp"

namespace clientapi {

/**
 * @class ClientAPIWrapper
 * @brief Singleton class that provides a clean API for the GUI to communicate with the server
 * 
 * This class is responsible for:
 * - Managing the connection to the server
 * - Providing methods to send commands to the server
 * - Handling server responses and events
 * - Maintaining a clean interface for the GUI
 * 
 * The API follows the Zappy GUI protocol specification exactly, with all messages
 * being handled through the Protocol class. This ensures consistent message
 * formatting and parsing across the application.
 * 
 * Usage example:
 * @code
 * // Get the singleton instance
 * auto& api = ClientAPIWrapper::getInstance();
 * 
 * // Connect to the server
 * api.connect("localhost", 4242);
 * 
 * // Register callbacks for server events
 * api.setMapSizeCallback([](int width, int height) {
 *     // Handle map size update
 * });
 * 
 * // Send commands to the server
 * api.sendMapSizeRequest();
 * @endcode
 */
class ClientAPIWrapper {
    public:
        /**
         * @brief Get the singleton instance of the ClientAPIWrapper
         * @return Reference to the singleton instance
         */
        static ClientAPIWrapper& getInstance();

        /**
         * @brief Connect to the Zappy server
         * @param host The server hostname or IP address
         * @param port The server port number
         * @return true if connection was successful, false otherwise
         */
        bool connect(const std::string& host, int port);

        /**
         * @brief Disconnect from the server
         */
        void disconnect();

        /**
         * @brief Check if currently connected to the server
         * @return true if connected, false otherwise
         */
        bool isConnected() const;

        // Server commands
        /**
         * @brief Request the map size from the server
         * @return true if request was sent successfully
         */
        bool sendMapSizeRequest();

        /**
         * @brief Request the content of a specific tile
         * @param x The x coordinate of the tile
         * @param y The y coordinate of the tile
         * @return true if request was sent successfully
         */
        bool sendTileContentRequest(int x, int y);

        /**
         * @brief Request the content of all tiles
         * @return true if request was sent successfully
         */
        bool sendMapContentRequest();

        /**
         * @brief Request the list of team names
         * @return true if request was sent successfully
         */
        bool sendTeamNamesRequest();

        /**
         * @brief Request a player's position
         * @param playerId The ID of the player
         * @return true if request was sent successfully
         */
        bool sendPlayerPositionRequest(int playerId);

        /**
         * @brief Request a player's level
         * @param playerId The ID of the player
         * @return true if request was sent successfully
         */
        bool sendPlayerLevelRequest(int playerId);

        /**
         * @brief Request a player's inventory
         * @param playerId The ID of the player
         * @return true if request was sent successfully
         */
        bool sendPlayerInventoryRequest(int playerId);

        /**
         * @brief Request the current time unit
         * @return true if request was sent successfully
         */
        bool sendTimeUnitRequest();

        /**
         * @brief Request to modify the time unit
         * @param timeUnit The new time unit value
         * @return true if request was sent successfully
         */
        bool sendTimeUnitModification(int timeUnit);

        /**
         * @brief Send the GRAPHIC command to identify as a GUI client
         * @return true if command was sent successfully
         */
        bool sendGraphicCommand();

        // Event callback types
        /**
         * @brief Callback for map size updates
         * @param width The map width
         * @param height The map height
         */
        using MapSizeCallback = std::function<void(int width, int height)>;

        /**
         * @brief Callback for tile content updates
         * @param x The tile x coordinate
         * @param y The tile y coordinate
         * @param resources Vector of resource quantities
         */
        using TileContentCallback = std::function<void(int x, int y, const std::vector<int>& resources)>;

        /**
         * @brief Callback for team names updates
         * @param teams Vector of team names
         */
        using TeamNamesCallback = std::function<void(const std::vector<std::string>& teams)>;

        /**
         * @brief Callback for new player events
         * @param id The player ID
         * @param x The player's x coordinate
         * @param y The player's y coordinate
         * @param orientation The player's orientation
         * @param level The player's level
         * @param team The player's team name
         */
        using PlayerNewCallback = std::function<void(int id, int x, int y, int orientation, int level, const std::string& team)>;

        /**
         * @brief Callback for player position updates
         * @param id The player ID
         * @param x The new x coordinate
         * @param y The new y coordinate
         * @param orientation The new orientation
         */
        using PlayerPositionCallback = std::function<void(int id, int x, int y, int orientation)>;

        /**
         * @brief Callback for player level updates
         * @param id The player ID
         * @param level The new level
         */
        using PlayerLevelCallback = std::function<void(int id, int level)>;

        /**
         * @brief Callback for player inventory updates
         * @param id The player ID
         * @param x The player's x coordinate
         * @param y The player's y coordinate
         * @param inventory Vector of inventory quantities
         */
        using PlayerInventoryCallback = std::function<void(int id, int x, int y, const std::array<int, ResourceType::COUNT>& inventory)>;

        /**
         * @brief Callback for player expulsion events
         * @param id The ID of the expelled player
         */
        using PlayerExpulsionCallback = std::function<void(int id)>;

        /**
         * @brief Callback for player broadcast messages
         * @param id The player ID
         * @param message The broadcast message
         */
        using PlayerBroadcastCallback = std::function<void(int id, const std::string& message)>;

        /**
         * @brief Callback for incantation start events
         * @param x The x coordinate of the incantation
         * @param y The y coordinate of the incantation
         * @param level The target level
         * @param players Vector of participating player IDs
         */
        using IncantationStartCallback = std::function<void(int x, int y, int level, const std::vector<int>& players)>;

        /**
         * @brief Callback for incantation end events
         * @param x The x coordinate of the incantation
         * @param y The y coordinate of the incantation
         * @param result The result of the incantation (0 for failure, 1 for success)
         */
        using IncantationEndCallback = std::function<void(int x, int y, int result)>;

        /**
         * @brief Callback for resource drop events
         * @param playerId The player ID
         * @param resourceId The resource ID
         */
        using ResourceDropCallback = std::function<void(int playerId, int resourceId)>;

        /**
         * @brief Callback for resource collection events
         * @param playerId The player ID
         * @param resourceId The resource ID
         */
        using ResourceCollectCallback = std::function<void(int playerId, int resourceId)>;

        /**
         * @brief Callback for player death events
         * @param id The ID of the dead player
         */
        using PlayerDeathCallback = std::function<void(int id)>;

        /**
         * @brief Callback for egg laying events
         * @param eggId The egg ID
         * @param playerId The player ID
         * @param x The x coordinate
         * @param y The y coordinate
         */
        using EggLaidCallback = std::function<void(int eggId, int playerId, int x, int y)>;

        /**
         * @brief Callback for egg hatching events
         * @param eggId The egg ID
         */
        using EggHatchingCallback = std::function<void(int eggId)>;

        /**
         * @brief Callback for egg death events
         * @param eggId The egg ID
         */
        using EggDeathCallback = std::function<void(int eggId)>;

        /**
         * @brief Callback for time unit updates
         * @param timeUnit The new time unit value
         */
        using TimeUnitCallback = std::function<void(int timeUnit)>;

        /**
         * @brief Callback for game end events
         * @param winningTeam The name of the winning team
         */
        using GameEndCallback = std::function<void(const std::string& winningTeam)>;

        /**
         * @brief Callback for server messages
         * @param message The server message
         */
        using ServerMessageCallback = std::function<void(const std::string& message)>;

        /**
         * @brief Callback for raw messages
         * @param message The raw message
         */
        using RawMessageCallback = std::function<void(const std::string&)>;

        // Event callback setters
        /**
         * @brief Set the callback for map size updates
         * @param callback The callback function
         */
        void setMapSizeCallback(MapSizeCallback callback);

        /**
         * @brief Set the callback for tile content updates
         * @param callback The callback function
         */
        void setTileContentCallback(TileContentCallback callback);

        /**
         * @brief Set the callback for team names updates
         * @param callback The callback function
         */
        void setTeamNamesCallback(TeamNamesCallback callback);

        /**
         * @brief Set the callback for new player events
         * @param callback The callback function
         */
        void setPlayerNewCallback(PlayerNewCallback callback);

        /**
         * @brief Set the callback for player position updates
         * @param callback The callback function
         */
        void setPlayerPositionCallback(PlayerPositionCallback callback);

        /**
         * @brief Set the callback for player level updates
         * @param callback The callback function
         */
        void setPlayerLevelCallback(PlayerLevelCallback callback);

        /**
         * @brief Set the callback for player inventory updates
         * @param callback The callback function
         */
        void setPlayerInventoryCallback(PlayerInventoryCallback callback);

        /**
         * @brief Set the callback for player expulsion events
         * @param callback The callback function
         */
        void setPlayerExpulsionCallback(PlayerExpulsionCallback callback);

        /**
         * @brief Set the callback for player broadcast messages
         * @param callback The callback function
         */
        void setPlayerBroadcastCallback(PlayerBroadcastCallback callback);

        /**
         * @brief Set the callback for incantation start events
         * @param callback The callback function
         */
        void setIncantationStartCallback(IncantationStartCallback callback);

        /**
         * @brief Set the callback for incantation end events
         * @param callback The callback function
         */
        void setIncantationEndCallback(IncantationEndCallback callback);

        /**
         * @brief Set the callback for resource drop events
         * @param callback The callback function
         */
        void setResourceDropCallback(ResourceDropCallback callback);

        /**
         * @brief Set the callback for resource collection events
         * @param callback The callback function
         */
        void setResourceCollectCallback(ResourceCollectCallback callback);

        /**
         * @brief Set the callback for player death events
         * @param callback The callback function
         */
        void setPlayerDeathCallback(PlayerDeathCallback callback);

        /**
         * @brief Set the callback for egg laying events
         * @param callback The callback function
         */
        void setEggLaidCallback(EggLaidCallback callback);

        /**
         * @brief Set the callback for egg hatching events
         * @param callback The callback function
         */
        void setEggHatchingCallback(EggHatchingCallback callback);

        /**
         * @brief Set the callback for egg death events
         * @param callback The callback function
         */
        void setEggDeathCallback(EggDeathCallback callback);

        /**
         * @brief Set the callback for time unit updates
         * @param callback The callback function
         */
        void setTimeUnitCallback(TimeUnitCallback callback);

        /**
         * @brief Set the callback for game end events
         * @param callback The callback function
         */
        void setGameEndCallback(GameEndCallback callback);

        /**
         * @brief Set the callback for server messages
         * @param callback The callback function
         */
        void setServerMessageCallback(ServerMessageCallback callback);

        /**
         * @brief Set the callback for raw messages
         * @param callback The callback function
         */
        void setRawMessageCallback(RawMessageCallback callback);

        GameState& getGameState();
        const GameState& getGameState() const;

        void onPlayerNew(int id, int x, int y, int orientation, int level, const std::string& team);
        void onPlayerPosition(int id, int x, int y, int orientation);
        void onPlayerLevel(int id, int level);
        void onPlayerInventory(int id, int x, int y, const std::array<int, ResourceType::COUNT>& inventory);
        void onMapSize(int width, int height);
        void onTileContent(int x, int y, const std::vector<int>& resources);

    private:
        /**
         * @brief Private constructor for singleton pattern
         */
        ClientAPIWrapper();

        /**
         * @brief Destructor
         */
        ~ClientAPIWrapper();

        // Prevent copying
        ClientAPIWrapper(const ClientAPIWrapper&) = delete;
        ClientAPIWrapper& operator=(const ClientAPIWrapper&) = delete;

        /**
         * @brief Handle server responses and dispatch to appropriate callbacks
         * @param response The server response to handle
         */
        void handleServerResponse(const std::string& response);

        // Network manager
        std::unique_ptr<NetworkManager> _networkManager;

        // Event callbacks
        MapSizeCallback _mapSizeCallback;
        TileContentCallback _tileContentCallback;
        TeamNamesCallback _teamNamesCallback;
        PlayerNewCallback _playerNewCallback;
        PlayerPositionCallback _playerPositionCallback;
        PlayerLevelCallback _playerLevelCallback;
        PlayerInventoryCallback _playerInventoryCallback;
        PlayerExpulsionCallback _playerExpulsionCallback;
        PlayerBroadcastCallback _playerBroadcastCallback;
        IncantationStartCallback _incantationStartCallback;
        IncantationEndCallback _incantationEndCallback;
        ResourceDropCallback _resourceDropCallback;
        ResourceCollectCallback _resourceCollectCallback;
        PlayerDeathCallback _playerDeathCallback;
        EggLaidCallback _eggLaidCallback;
        EggHatchingCallback _eggHatchingCallback;
        EggDeathCallback _eggDeathCallback;
        TimeUnitCallback _timeUnitCallback;
        GameEndCallback _gameEndCallback;
        ServerMessageCallback _serverMessageCallback;
        RawMessageCallback _rawMessageCallback;
        GameState _gameState;
};

} // namespace clientapi

#endif /* !CLIENTAPIWRAPPER_HPP_ */ 