/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientHandler - Manages individual client connections and protocol handling
*/

#include "ClientHandler.hpp"
#include "ConnectionManager.hpp"
#include "Protocol.hpp"
#include "../GameManager.hpp"
#include "../Model/Player.hpp"
#include "../Model/Team.hpp"
#include "../Model/ResourceType.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <errno.h>
#include <cstring>

namespace clientapi {

ClientHandler::ClientHandler(int socket, ConnectionManager* manager)
    : _socket(socket), _connectionManager(manager), _clientType(ClientType::UNKNOWN)
{
    _commandBuffer = std::make_unique<clientapi::CommandBuffer>();
}

ClientHandler::~ClientHandler() {
    if (_socket >= 0) {
        close(_socket);
    }
}

bool ClientHandler::handleIncomingData() {
    if (!readFromSocket()) {
        return false;
    }
    
    processBufferedInput();
    return true;
}

bool ClientHandler::readFromSocket() {
    char buffer[1024];
    ssize_t bytesRead = recv(_socket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead <= 0) {
        return false;
    }
    
    buffer[bytesRead] = '\0';
    _inputBuffer += buffer;
    return true;
}

void ClientHandler::processBufferedInput() {
    size_t pos;
    while ((pos = _inputBuffer.find('\n')) != std::string::npos) {
        std::string line = _inputBuffer.substr(0, pos);
        _inputBuffer.erase(0, pos + 1);
        
        // Remove carriage return if present
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        if (!isAuthenticated()) {
            handleAuthenticationResponse(line);
        } else {
            processCommand(line);
        }
    }
}

void ClientHandler::sendWelcome() {
    sendResponse("WELCOME");
}

void ClientHandler::sendResponse(const std::string& response) {
    std::string message = response;
    if (!message.empty() && message.back() != '\n') {
        message += '\n';
    }
    
    ssize_t bytesSent = send(_socket, message.c_str(), message.length(), 0);
    if (bytesSent < 0) {
        std::cerr << "Error sending response to client " << _socket << ": " << strerror(errno) << std::endl;
    } else if (bytesSent != static_cast<ssize_t>(message.length())) {
        std::cerr << "Warning: Partial send to client " << _socket << " (" << bytesSent << "/" << message.length() << " bytes)" << std::endl;
    }
}

void ClientHandler::sendMapSize(int width, int height) {
    std::string message = "msz " + std::to_string(width) + " " + std::to_string(height) + "\n";
    sendResponse(message);
}

void ClientHandler::sendTileContent(int x, int y, const std::vector<int>& resources) {
    if (resources.size() != 7) {
        std::cerr << "Error: Tile content must have exactly 7 resources" << std::endl;
        return;
    }
    
    std::string message = "bct " + std::to_string(x) + " " + std::to_string(y);
    for (int resource : resources) {
        message += " " + std::to_string(resource);
    }
    message += "\n";
    sendResponse(message);
}

void ClientHandler::sendAllTileContents(const std::vector<std::vector<int>>& allTiles) {
    // Send mct command to request all tiles
    sendResponse("mct\n");
}

void ClientHandler::sendTeamNames(const std::vector<std::string>& teams) {
    for (const std::string& team : teams) {
        std::string message = "tna " + team + "\n";
        sendResponse(message);
    }
}

void ClientHandler::sendPlayerPosition(int playerId, int x, int y, int orientation) {
    std::string message = "ppo #" + std::to_string(playerId) + " " + 
        std::to_string(x) + " " + std::to_string(y) + " " + 
        std::to_string(orientation) + "\n";
    sendResponse(message);
}

void ClientHandler::sendPlayerLevel(int playerId, int level) {
    std::string message = "plv #" + std::to_string(playerId) + " " + std::to_string(level) + "\n";
    sendResponse(message);
}

void ClientHandler::sendPlayerInventory(int playerId, int x, int y, const std::vector<int>& inventory) {
    if (inventory.size() != 7) {
        std::cerr << "Error: Inventory must have exactly 7 resources" << std::endl;
        return;
    }
    
    std::string message = "pin #" + std::to_string(playerId) + " " + 
        std::to_string(x) + " " + std::to_string(y);
    for (int resource : inventory) {
        message += " " + std::to_string(resource);
    }
    message += "\n";
    sendResponse(message);
}

void ClientHandler::sendTimeUnit(int timeUnit) {
    std::string message = "sgt " + std::to_string(timeUnit) + "\n";
    sendResponse(message);
}

void ClientHandler::sendPlayerNew(int playerId, int x, int y, int orientation, int level, const std::string& team) {
    std::string message = "pnw #" + std::to_string(playerId) + " " + 
        std::to_string(x) + " " + std::to_string(y) + " " + 
        std::to_string(orientation) + " " + std::to_string(level) + " " + 
        team + "\n";
    sendResponse(message);
}

void ClientHandler::sendPlayerExpulsion(int playerId) {
    std::string message = "pex #" + std::to_string(playerId) + "\n";
    sendResponse(message);
}

void ClientHandler::sendPlayerBroadcast(int playerId, const std::string& message) {
    std::string broadcast = "pbc #" + std::to_string(playerId) + " " + message + "\n";
    sendResponse(broadcast);
}

void ClientHandler::sendIncantationStart(int x, int y, int level, const std::vector<int>& players) {
    std::string message = "pic " + std::to_string(x) + " " + std::to_string(y) + " " + 
        std::to_string(level);
    for (int playerId : players) {
        message += " #" + std::to_string(playerId);
    }
    message += "\n";
    sendResponse(message);
}

void ClientHandler::sendIncantationEnd(int x, int y, int result) {
    std::string message = "pie " + std::to_string(x) + " " + std::to_string(y) + " " + 
        std::to_string(result) + "\n";
    sendResponse(message);
}

void ClientHandler::sendResourceDrop(int playerId, int resourceId) {
    std::string message = "pdr #" + std::to_string(playerId) + " " + std::to_string(resourceId) + "\n";
    sendResponse(message);
}

void ClientHandler::sendResourceCollect(int playerId, int resourceId) {
    std::string message = "pgt #" + std::to_string(playerId) + " " + std::to_string(resourceId) + "\n";
    sendResponse(message);
}

void ClientHandler::sendPlayerDeath(int playerId) {
    std::string message = "pdi #" + std::to_string(playerId) + "\n";
    sendResponse(message);
}

void ClientHandler::sendEggLaid(int eggId, int playerId, int x, int y) {
    std::string message = "enw #" + std::to_string(eggId) + " #" + std::to_string(playerId) + " " + 
        std::to_string(x) + " " + std::to_string(y) + "\n";
    sendResponse(message);
}

void ClientHandler::sendEggHatching(int eggId) {
    std::string message = "eht #" + std::to_string(eggId) + "\n";
    sendResponse(message);
}

void ClientHandler::sendEggDeath(int eggId) {
    std::string message = "edi #" + std::to_string(eggId) + "\n";
    sendResponse(message);
}

void ClientHandler::sendGameEnd(const std::string& winningTeam) {
    std::string message = "seg " + winningTeam + "\n";
    sendResponse(message);
}

void ClientHandler::sendServerMessage(const std::string& message) {
    std::string serverMsg = "smg " + message + "\n";
    sendResponse(serverMsg);
}

bool ClientHandler::authenticate(const std::string& teamName) {
    if (teamName == "GRAPHIC") {
        _clientType = ClientType::GRAPHIC;
        _teamName = teamName;
        
        // Register this GUI client with the GameManager
        GameManager::getInstance().registerGUIClient(this);
        
        sendGameInfo();
        return true;
    }
    
    if (_connectionManager->isValidTeam(teamName)) {
        int availableSlots = _connectionManager->getAvailableSlots(teamName);
        if (availableSlots > 0) {
            _clientType = ClientType::AI;
            _teamName = teamName;
            _player = std::make_unique<Player>(0, teamName, Position(0, 0));
            _player->setSocket(_socket);
            
            sendGameInfo();
            return true;
        }
    }
    
    return false;
}

void ClientHandler::handleAuthenticationResponse(const std::string& input) {
    if (authenticate(input)) {
        std::cout << "Client authenticated as " << _teamName << std::endl;
    } else {
        std::cout << "Authentication failed for: " << input << std::endl;
        // Could close connection here or send error
    }
}

void ClientHandler::sendGameInfo() {
    if (_clientType == ClientType::AI) {
        // For AI clients: "X Y\n" followed by number of available slots
        auto [width, height] = GameManager::getInstance().getMapSize();
        sendResponse(std::to_string(width) + " " + std::to_string(height));
        int slots = _connectionManager->getAvailableSlots(_teamName);
        sendResponse(std::to_string(slots));
    } else if (_clientType == ClientType::GRAPHIC) {
        // For GUI clients: Send complete initial game state
        // This should include map size, all tiles, all players, teams, etc.
        auto [width, height] = GameManager::getInstance().getMapSize();
        sendMapSize(width, height);
        
        // Send all tile contents
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                sendTileContent(x, y, GameManager::getInstance().getTileContent(x, y));
            }
        }
        
        // Send team names
        sendTeamNames(GameManager::getInstance().getTeamNames());
        
        // Send all players
        auto players = GameManager::getInstance().getAllPlayers();
        for (Player* player : players) {
            sendPlayerNew(
                player->getId(),
                player->getPosition().x,
                player->getPosition().y,
                player->getOrientation(),
                player->getLevel(),
                player->getTeam()
            );
        }
        
        // Send current time unit
        sendTimeUnit(GameManager::getInstance().getTimeUnit());
    }
}

void ClientHandler::processCommand(const std::string& command) {
    if (_clientType == ClientType::AI && _player) {
        // AI clients: Add commands to buffer for timed execution
        _commandBuffer->addCommand(command, _player.get());
    } else if (_clientType == ClientType::GRAPHIC) {
        // GUI clients: Process commands immediately and send responses
        // GUI commands are different from AI commands (msz, bct, mct, etc.)
        processGUICommand(command);
    }
}

void ClientHandler::processGUICommand(const std::string& command) {
    // Parse GUI command and send appropriate response
    std::vector<std::string> parts = Protocol::split(command, ' ');
    if (parts.empty()) {
        sendResponse("suc");
        return;
    }
    
    std::string cmd = parts[0];
    
    if (cmd == "msz") {
        // Request map size
        auto [width, height] = GameManager::getInstance().getMapSize();
        sendMapSize(width, height);
    } else if (cmd == "bct" && parts.size() >= 3) {
        // Request tile content: bct X Y
        int x = std::stoi(parts[1]);
        int y = std::stoi(parts[2]);
        auto tileContent = GameManager::getInstance().getTileContent(x, y);
        sendTileContent(x, y, tileContent);
    } else if (cmd == "mct") {
        // Request all tiles content
        auto allTiles = GameManager::getInstance().getAllTileContents();
        auto [width, height] = GameManager::getInstance().getMapSize();
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                sendTileContent(x, y, GameManager::getInstance().getTileContent(x, y));
            }
        }
    } else if (cmd == "tna") {
        // Request team names
        sendTeamNames(GameManager::getInstance().getTeamNames());
    } else if (cmd == "ppo" && parts.size() >= 2) {
        // Request player position: ppo #n
        int playerId = std::stoi(parts[1].substr(1)); // Remove '#'
        Player* player = GameManager::getInstance().getPlayer(playerId);
        if (player) {
            sendPlayerPosition(playerId, player->getPosition().x, player->getPosition().y, player->getOrientation());
        } else {
            sendResponse("sbp");
        }
    } else if (cmd == "plv" && parts.size() >= 2) {
        // Request player level: plv #n
        int playerId = std::stoi(parts[1].substr(1)); // Remove '#'
        Player* player = GameManager::getInstance().getPlayer(playerId);
        if (player) {
            sendPlayerLevel(playerId, player->getLevel());
        } else {
            sendResponse("sbp");
        }
    } else if (cmd == "pin" && parts.size() >= 2) {
        // Request player inventory: pin #n
        int playerId = std::stoi(parts[1].substr(1)); // Remove '#'
        Player* player = GameManager::getInstance().getPlayer(playerId);
        if (player) {
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
            sendPlayerInventory(playerId, player->getPosition().x, player->getPosition().y, inventory);
        } else {
            sendResponse("sbp");
        }
    } else if (cmd == "sgt") {
        // Request time unit
        sendTimeUnit(GameManager::getInstance().getTimeUnit());
    } else if (cmd == "sst" && parts.size() >= 2) {
        // Modify time unit: sst T
        int timeUnit = std::stoi(parts[1]);
        GameManager::getInstance().setTimeUnit(timeUnit);
        sendTimeUnit(timeUnit);
    } else {
        // Unknown command
        sendResponse("suc");
    }
}

} // namespace clientapi 