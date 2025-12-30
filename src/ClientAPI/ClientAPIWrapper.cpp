/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientAPIWrapper - Main game client API wrapper controller
*/

#include "ClientAPIWrapper.hpp"
#include <iostream>
#include "Model/ResourceType.hpp"
using clientapi::ResourceType;

namespace clientapi {

/**
 * @brief Get the singleton instance of the ClientAPIWrapper
 * @return Reference to the singleton instance
 */
ClientAPIWrapper& ClientAPIWrapper::getInstance() {
    static ClientAPIWrapper instance;
    return instance;
}

/**
 * @brief Construct a new ClientAPIWrapper object
 * 
 * Initializes the network manager and sets up the response callback
 * to handle incoming server messages.
 */
ClientAPIWrapper::ClientAPIWrapper()
    : _networkManager(std::make_unique<NetworkManager>())
{
    _networkManager->setResponseCallback([this](const std::string& response) {
        if (_rawMessageCallback) _rawMessageCallback(response);
        handleServerResponse(response);
    });
}

/**
 * @brief Destroy the ClientAPIWrapper object
 */
ClientAPIWrapper::~ClientAPIWrapper() = default;

/**
 * @brief Connect to the Zappy server
 * @param host The server hostname or IP address
 * @param port The server port number
 * @return true if connection was successful, false otherwise
 */
bool ClientAPIWrapper::connect(const std::string& host, int port) {
    return _networkManager->connect(host, port);
}

/**
 * @brief Disconnect from the server
 */
void ClientAPIWrapper::disconnect() {
    _networkManager->disconnect();
}

/**
 * @brief Check if currently connected to the server
 * @return true if connected, false otherwise
 */
bool ClientAPIWrapper::isConnected() const {
    return _networkManager->isConnected();
}

// Server commands
/**
 * @brief Request the map size from the server
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendMapSizeRequest() {
    return _networkManager->sendCommand(Protocol::formatMapSizeRequest());
}

/**
 * @brief Request the content of a specific tile
 * @param x The x coordinate of the tile
 * @param y The y coordinate of the tile
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendTileContentRequest(int x, int y) {
    return _networkManager->sendCommand(Protocol::formatTileContentRequest(x, y));
}

/**
 * @brief Request the content of all tiles
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendMapContentRequest() {
    return _networkManager->sendCommand(Protocol::formatMapContentRequest());
}

/**
 * @brief Request the list of team names
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendTeamNamesRequest() {
    return _networkManager->sendCommand(Protocol::formatTeamNamesRequest());
}

/**
 * @brief Request a player's position
 * @param playerId The ID of the player
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendPlayerPositionRequest(int playerId) {
    return _networkManager->sendCommand(Protocol::formatPlayerPositionRequest(playerId));
}

/**
 * @brief Request a player's level
 * @param playerId The ID of the player
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendPlayerLevelRequest(int playerId) {
    return _networkManager->sendCommand(Protocol::formatPlayerLevelRequest(playerId));
}

/**
 * @brief Request a player's inventory
 * @param playerId The ID of the player
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendPlayerInventoryRequest(int playerId) {
    return _networkManager->sendCommand(Protocol::formatPlayerInventoryRequest(playerId));
}

/**
 * @brief Request the current time unit
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendTimeUnitRequest() {
    return _networkManager->sendCommand(Protocol::formatTimeUnitRequest());
}

/**
 * @brief Request to modify the time unit
 * @param timeUnit The new time unit value
 * @return true if request was sent successfully
 */
bool ClientAPIWrapper::sendTimeUnitModification(int timeUnit) {
    return _networkManager->sendCommand(Protocol::formatTimeUnitModification(timeUnit));
}

/**
 * @brief Send the GRAPHIC command to identify as a GUI client
 * @return true if command was sent successfully
 */
bool ClientAPIWrapper::sendGraphicCommand() {
    return _networkManager->sendCommand("GRAPHIC\n");
}

// Event callback setters
/**
 * @brief Set the callback for map size updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setMapSizeCallback(MapSizeCallback callback) {
    _mapSizeCallback = callback;
}

/**
 * @brief Set the callback for tile content updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setTileContentCallback(TileContentCallback callback) {
    _tileContentCallback = callback;
}

/**
 * @brief Set the callback for team names updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setTeamNamesCallback(TeamNamesCallback callback) {
    _teamNamesCallback = callback;
}

/**
 * @brief Set the callback for new player events
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerNewCallback(PlayerNewCallback callback) {
    _playerNewCallback = callback;
}

/**
 * @brief Set the callback for player position updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerPositionCallback(PlayerPositionCallback callback) {
    _playerPositionCallback = callback;
}

/**
 * @brief Set the callback for player level updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerLevelCallback(PlayerLevelCallback callback) {
    _playerLevelCallback = callback;
}

/**
 * @brief Set the callback for player inventory updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerInventoryCallback(PlayerInventoryCallback callback) {
    _playerInventoryCallback = callback;
}

/**
 * @brief Set the callback for player expulsion events
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerExpulsionCallback(PlayerExpulsionCallback callback) {
    _playerExpulsionCallback = callback;
}

/**
 * @brief Set the callback for player broadcast messages
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerBroadcastCallback(PlayerBroadcastCallback callback) {
    _playerBroadcastCallback = callback;
}

/**
 * @brief Set the callback for incantation start events
 * @param callback The callback function
 */
void ClientAPIWrapper::setIncantationStartCallback(IncantationStartCallback callback) {
    _incantationStartCallback = callback;
}

/**
 * @brief Set the callback for incantation end events
 * @param callback The callback function
 */
void ClientAPIWrapper::setIncantationEndCallback(IncantationEndCallback callback) {
    _incantationEndCallback = callback;
}

/**
 * @brief Set the callback for resource drop events
 * @param callback The callback function
 */
void ClientAPIWrapper::setResourceDropCallback(ResourceDropCallback callback) {
    _resourceDropCallback = callback;
}

/**
 * @brief Set the callback for resource collection events
 * @param callback The callback function
 */
void ClientAPIWrapper::setResourceCollectCallback(ResourceCollectCallback callback) {
    _resourceCollectCallback = callback;
}

/**
 * @brief Set the callback for player death events
 * @param callback The callback function
 */
void ClientAPIWrapper::setPlayerDeathCallback(PlayerDeathCallback callback) {
    _playerDeathCallback = callback;
}

/**
 * @brief Set the callback for egg laying events
 * @param callback The callback function
 */
void ClientAPIWrapper::setEggLaidCallback(EggLaidCallback callback) {
    _eggLaidCallback = callback;
}

/**
 * @brief Set the callback for egg hatching events
 * @param callback The callback function
 */
void ClientAPIWrapper::setEggHatchingCallback(EggHatchingCallback callback) {
    _eggHatchingCallback = callback;
}

/**
 * @brief Set the callback for egg death events
 * @param callback The callback function
 */
void ClientAPIWrapper::setEggDeathCallback(EggDeathCallback callback) {
    _eggDeathCallback = callback;
}

/**
 * @brief Set the callback for time unit updates
 * @param callback The callback function
 */
void ClientAPIWrapper::setTimeUnitCallback(TimeUnitCallback callback) {
    _timeUnitCallback = callback;
}

/**
 * @brief Set the callback for game end events
 * @param callback The callback function
 */
void ClientAPIWrapper::setGameEndCallback(GameEndCallback callback) {
    _gameEndCallback = callback;
}

/**
 * @brief Set the callback for server messages
 * @param callback The callback function
 */
void ClientAPIWrapper::setServerMessageCallback(ServerMessageCallback callback) {
    _serverMessageCallback = callback;
}

/**
 * @brief Set the callback for raw messages
 * @param callback The callback function
 */
void ClientAPIWrapper::setRawMessageCallback(RawMessageCallback callback) {
    _rawMessageCallback = callback;
}

/**
 * @brief Handle server responses and dispatch to appropriate callbacks
 * 
 * This method is called for each complete message received from the server.
 * It parses the message using the Protocol class and calls the appropriate
 * callback function if one is registered.
 * 
 * @param response The server response to handle
 */
void ClientAPIWrapper::handleServerResponse(const std::string& response) {
    std::vector<std::string> parts = Protocol::split(response, ' ');
    if (parts.empty()) {
        return;
    }

    const std::string& command = parts[0];

    if (command == Protocol::MSG_MAP_SIZE) {
        int width, height;
        if (Protocol::parseMapSize(response, width, height)) {
            onMapSize(width, height);
            if (_mapSizeCallback) {
                _mapSizeCallback(width, height);
            }
        }
    }
    else if (command == Protocol::MSG_TILE_CONTENT) {
        int x, y;
        std::vector<int> resources;
        if (Protocol::parseTileContent(response, x, y, resources)) {
            onTileContent(x, y, resources);
            if (_tileContentCallback) {
                _tileContentCallback(x, y, resources);
            }
        }
    }
    else if (command == Protocol::MSG_TEAM_NAMES) {
        if (_teamNamesCallback) {
            std::vector<std::string> teams;
            for (size_t i = 1; i < parts.size(); ++i) {
                teams.push_back(parts[i]);
            }
            _teamNamesCallback(teams);
        }
    }
    else if (command == Protocol::MSG_PLAYER_NEW) {
        int id, x, y, orientation, level;
        std::string team;
        if (Protocol::parsePlayerNew(response, id, x, y, orientation, level, team) && _playerNewCallback) {
            onPlayerNew(id, x, y, orientation, level, team);
            if (_playerNewCallback) {
                _playerNewCallback(id, x, y, orientation, level, team);
            }
        }
    }
    else if (command == Protocol::MSG_PLAYER_POSITION) {
        int id, x, y, orientation;
        if (Protocol::parsePlayerPosition(response, id, x, y, orientation) && _playerPositionCallback) {
            _playerPositionCallback(id, x, y, orientation);
        }
    }
    else if (command == Protocol::MSG_PLAYER_LEVEL) {
        int id, level;
        if (Protocol::parsePlayerLevel(response, id, level) && _playerLevelCallback) {
            _playerLevelCallback(id, level);
        }
    }
    else if (command == Protocol::MSG_PLAYER_INVENTORY) {
        int id, x, y;
        std::vector<int> inventory;
        if (Protocol::parsePlayerInventory(response, id, x, y, inventory) && _playerInventoryCallback) {
            std::array<int, ResourceType::COUNT> arr{};
            std::copy_n(inventory.begin(), std::min(inventory.size(), arr.size()), arr.begin());
            _playerInventoryCallback(id, x, y, arr);
        }
    }
    else if (command == Protocol::MSG_PLAYER_EXPULSION) {
        int id;
        if (Protocol::parsePlayerExpulsion(response, id) && _playerExpulsionCallback) {
            _playerExpulsionCallback(id);
        }
    }
    else if (command == Protocol::MSG_PLAYER_BROADCAST) {
        int id;
        std::string message;
        if (Protocol::parsePlayerBroadcast(response, id, message) && _playerBroadcastCallback) {
            _playerBroadcastCallback(id, message);
        }
    }
    else if (command == Protocol::MSG_INCANTATION_START) {
        int x, y, level;
        std::vector<int> players;
        if (Protocol::parseIncantationStart(response, x, y, level, players) && _incantationStartCallback) {
            _incantationStartCallback(x, y, level, players);
        }
    }
    else if (command == Protocol::MSG_INCANTATION_END) {
        int x, y, result;
        if (Protocol::parseIncantationEnd(response, x, y, result) && _incantationEndCallback) {
            _incantationEndCallback(x, y, result);
        }
    }
    else if (command == Protocol::MSG_RESOURCE_DROP) {
        int playerId, resourceId;
        if (Protocol::parseResourceDrop(response, playerId, resourceId) && _resourceDropCallback) {
            _resourceDropCallback(playerId, resourceId);
        }
    }
    else if (command == Protocol::MSG_RESOURCE_COLLECT) {
        int playerId, resourceId;
        if (Protocol::parseResourceCollect(response, playerId, resourceId) && _resourceCollectCallback) {
            _resourceCollectCallback(playerId, resourceId);
        }
    }
    else if (command == Protocol::MSG_PLAYER_DEATH) {
        int id;
        if (Protocol::parsePlayerDeath(response, id) && _playerDeathCallback) {
            _playerDeathCallback(id);
        }
    }
    else if (command == Protocol::MSG_EGG_LAID) {
        int eggId, playerId, x, y;
        if (Protocol::parseEggLaid(response, eggId, playerId, x, y) && _eggLaidCallback) {
            _eggLaidCallback(eggId, playerId, x, y);
        }
    }
    else if (command == Protocol::MSG_EGG_HATCHING) {
        int eggId;
        if (Protocol::parseEggHatching(response, eggId) && _eggHatchingCallback) {
            _eggHatchingCallback(eggId);
        }
    }
    else if (command == Protocol::MSG_EGG_DEATH) {
        int eggId;
        if (Protocol::parseEggDeath(response, eggId) && _eggDeathCallback) {
            _eggDeathCallback(eggId);
        }
    }
    else if (command == Protocol::MSG_TIME_UNIT) {
        int timeUnit;
        if (Protocol::parseTimeUnit(response, timeUnit) && _timeUnitCallback) {
            _timeUnitCallback(timeUnit);
        }
    }
    else if (command == Protocol::MSG_GAME_END) {
        std::string winningTeam;
        if (Protocol::parseGameEnd(response, winningTeam) && _gameEndCallback) {
            _gameEndCallback(winningTeam);
        }
    }
    else if (command == Protocol::MSG_SERVER_MESSAGE) {
        std::string message;
        if (Protocol::parseServerMessage(response, message) && _serverMessageCallback) {
            _serverMessageCallback(message);
        }
    }
}

GameState& ClientAPIWrapper::getGameState() {
    return _gameState;
}

const GameState& ClientAPIWrapper::getGameState() const {
    return _gameState;
}

// Example handler for pnw (new player)
void ClientAPIWrapper::onPlayerNew(int id, int x, int y, int orientation, int level, const std::string& team) {
    Player player(id, team, {x, y});
    player.setOrientation(orientation);
    player.setLevel(level);
    _gameState.addOrUpdatePlayer(player);
    // Also update the team
    auto* t = _gameState.getTeam(team);
    if (!t) {
        Team newTeam(team);
        newTeam.addPlayer(id);
        _gameState.addOrUpdateTeam(newTeam);
    } else {
        t->addPlayer(id);
    }
}

// Example handler for ppo (player position update)
void ClientAPIWrapper::onPlayerPosition(int id, int x, int y, int orientation) {
    auto* player = _gameState.getPlayer(id);
    if (player) {
        player->setPosition({x, y});
        player->setOrientation(orientation);
    }
}

// Example handler for plv (player level update)
void ClientAPIWrapper::onPlayerLevel(int id, int level) {
    auto* player = _gameState.getPlayer(id);
    if (player) {
        player->setLevel(level);
    }
}

// Example handler for pin (player inventory update)
void ClientAPIWrapper::onPlayerInventory(int id, int x, int y, const std::array<int, ResourceType::COUNT>& inventory) {
    auto* player = _gameState.getPlayer(id);
    if (player) {
        player->setPosition({x, y});
        player->setInventory(inventory);
    }
}

// Handler for msz (map size)
void ClientAPIWrapper::onMapSize(int width, int height) {
    Map map(width, height);
    std::cout << "Map size set to: " << width << "x" << height << std::endl;
    _gameState.setMap(map);
}

// Handler for bct (tile content)
void ClientAPIWrapper::onTileContent(int x, int y, const std::vector<int>& resources) {
    auto* map = _gameState.getMap();
    if (map) {
        auto* tile = map->getTile(x, y);
        if (tile) {
            std::cout << "Tile content set to: " << x << "," << y << std::endl;
            // Convert vector to array for Tile::setResources
            std::array<int, ResourceType::COUNT> resourceArray{};
            for (size_t i = 0; i < resources.size() && i < resourceArray.size(); ++i) {
                resourceArray[i] = resources[i];
            }
            tile->setResources(resourceArray);
            std::cout << "tile has the following resources: " << std::endl;
            for (int i = 0; i < resourceArray.size(); ++i) {
                std::cout << resourceArray[i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout <<" full map is:" << std::endl;
        for (int y = 0; y < map->getHeight(); ++y) {
            for (int x = 0; x < map->getWidth(); ++x) {
                std::cout << map->getTile(x, y)->getResources()[0] << " ";
            }
            std::cout << std::endl;
        }
    }
}

} // namespace clientapi 
