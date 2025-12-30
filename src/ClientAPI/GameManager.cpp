/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameManager - Manages game state and coordinates all game systems
*/

#include "GameManager.hpp"
#include "Network/ClientHandler.hpp"
#include "Model/ResourceType.hpp"
#include "Model/Player.hpp"
#include "Model/Team.hpp"
#include "Model/GameState.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

namespace clientapi {

GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

GameManager::GameManager() : _gameState(clientapi::State::WAITING), _map(nullptr), _timeUnit(100) {
    // Initialize game manager
}

GameManager::~GameManager() {
    // Cleanup
}

std::pair<int, int> GameManager::getMapSize() const {
    if (_map) {
        return std::make_pair(_map->getWidth(), _map->getHeight());
    }
    // Default size if no map is set
    return std::make_pair(20, 20);
}

std::vector<int> GameManager::getTileContent(int x, int y) const {
    if (_map) {
        // TODO: Get actual tile content from map
        // This would require the Map class to have a getTileContent method
        return {1, 0, 0, 0, 0, 0, 0}; // Placeholder
    }
    return {1, 0, 0, 0, 0, 0, 0}; // Default tile content
}

std::vector<std::vector<int>> GameManager::getAllTileContents() const {
    std::vector<std::vector<int>> allTiles;
    auto [width, height] = getMapSize();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            allTiles.push_back(getTileContent(x, y));
        }
    }
    return allTiles;
}

std::vector<std::string> GameManager::getTeamNames() const {
    std::vector<std::string> names;
    // TODO: Implement when Team class is available
    // For now, return placeholder names
    names.push_back("team1");
    names.push_back("team2");
    return names;
}

int GameManager::getTimeUnit() const {
    return _timeUnit;
}

void GameManager::setTimeUnit(int timeUnit) {
    _timeUnit = timeUnit;
    // Broadcast time unit change to all GUI clients
    broadcastToGUI("sgt " + std::to_string(timeUnit));
}

std::vector<Player*> GameManager::getAllPlayers() const {
    std::vector<Player*> players;
    // TODO: Implement when Team class is available
    // For now, return empty vector
    return players;
}

Player* GameManager::getPlayer(int playerId) const {
    // TODO: Implement when Team class is available
    // For now, return nullptr
    return nullptr;
}

void GameManager::broadcastToGUI(const std::string& event) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            // Send the event to this GUI client
            // The event should already be formatted according to GUI protocol
            guiClient->sendResponse(event);
        }
    }
}

void GameManager::registerGUIClient(ClientHandler* clientHandler) {
    if (clientHandler && clientHandler->getClientType() == ClientType::GRAPHIC) {
        _guiClients.push_back(clientHandler);
        
        // Send initial game state to the new GUI client
        sendInitialGameState(clientHandler);
    }
}

void GameManager::unregisterGUIClient(ClientHandler* clientHandler) {
    _guiClients.erase(
        std::remove(_guiClients.begin(), _guiClients.end(), clientHandler),
        _guiClients.end()
    );
}

void GameManager::sendInitialGameState(ClientHandler* guiClient) {
    // Send complete initial game state to new GUI client
    auto [width, height] = getMapSize();
    guiClient->sendMapSize(width, height);
    
    // Send all tile contents
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            guiClient->sendTileContent(x, y, getTileContent(x, y));
        }
    }
    
    // Send team names
    guiClient->sendTeamNames(getTeamNames());
    
    // Send all players
    auto players = getAllPlayers();
    for (Player* player : players) {
        guiClient->sendPlayerNew(
            player->getId(),
            player->getPosition().x,
            player->getPosition().y,
            player->getOrientation(),
            player->getLevel(),
            player->getTeamName()
        );
    }
    
    // Send current time unit
    guiClient->sendTimeUnit(getTimeUnit());
}

// Event broadcasting methods for game events
void GameManager::broadcastPlayerNew(Player* player) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendPlayerNew(
                player->getId(),
                player->getPosition().x,
                player->getPosition().y,
                player->getOrientation(),
                player->getLevel(),
                player->getTeamName()
            );
        }
    }
}

void GameManager::broadcastPlayerPosition(Player* player) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendPlayerPosition(
                player->getId(),
                player->getPosition().x,
                player->getPosition().y,
                player->getOrientation()
            );
        }
    }
}

void GameManager::broadcastPlayerLevel(Player* player) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendPlayerLevel(player->getId(), player->getLevel());
        }
    }
}

void GameManager::broadcastPlayerInventory(Player* player) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            // Get actual inventory from player
            std::vector<int> inventory = {
                player->getFood(),
                player->getResourceCount(ResourceType::LINEMATE),
                player->getResourceCount(ResourceType::DERAUMERE),
                player->getResourceCount(ResourceType::SIBUR),
                player->getResourceCount(ResourceType::MENDIANE),
                player->getResourceCount(ResourceType::PHIRAS),
                player->getResourceCount(ResourceType::THYSTAME)
            };
            guiClient->sendPlayerInventory(
                player->getId(),
                player->getPosition().x,
                player->getPosition().y,
                inventory
            );
        }
    }
}

void GameManager::broadcastPlayerDeath(Player* player) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendPlayerDeath(player->getId());
        }
    }
}

void GameManager::broadcastPlayerExpulsion(Player* player) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendPlayerExpulsion(player->getId());
        }
    }
}

void GameManager::broadcastPlayerBroadcast(Player* player, const std::string& message) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendPlayerBroadcast(player->getId(), message);
        }
    }
}

void GameManager::broadcastIncantationStart(int x, int y, int level, const std::vector<int>& players) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendIncantationStart(x, y, level, players);
        }
    }
}

void GameManager::broadcastIncantationEnd(int x, int y, int result) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendIncantationEnd(x, y, result);
        }
    }
}

void GameManager::broadcastResourceDrop(Player* player, int resourceId) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendResourceDrop(player->getId(), resourceId);
        }
    }
}

void GameManager::broadcastResourceCollect(Player* player, int resourceId) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendResourceCollect(player->getId(), resourceId);
        }
    }
}

void GameManager::broadcastEggLaid(int eggId, Player* player, int x, int y) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendEggLaid(eggId, player->getId(), x, y);
        }
    }
}

void GameManager::broadcastEggHatching(int eggId) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendEggHatching(eggId);
        }
    }
}

void GameManager::broadcastEggDeath(int eggId) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendEggDeath(eggId);
        }
    }
}

void GameManager::broadcastGameEnd(const std::string& winningTeam) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendGameEnd(winningTeam);
        }
    }
}

void GameManager::broadcastServerMessage(const std::string& message) {
    for (ClientHandler* guiClient : _guiClients) {
        if (guiClient && guiClient->getClientType() == ClientType::GRAPHIC) {
            guiClient->sendServerMessage(message);
        }
    }
}

} // namespace clientapi 