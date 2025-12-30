/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CommandBuffer
*/

#include "CommandBuffer.hpp"
#include "Protocol.hpp"
#include "../Model/Player.hpp"
#include "../Model/ResourceType.hpp"
using clientapi::Player;
using clientapi::ResourceType;
using clientapi::resourceTypeToString;
#include <iostream>

namespace clientapi {

CommandBuffer::CommandBuffer() {
}

bool CommandBuffer::addCommand(const std::string& command, Player* player) {
    if (isFull()) {
        return false;
    }
    
    if (!Protocol::isValidCommand(command)) {
        // Send "ko" response immediately for invalid commands
        // TODO: Send response through connection manager
        return false;
    }
    
    int timeLimit = Protocol::getCommandTimeLimit(command);
    _commandQueue.emplace(command, player, timeLimit);
    return true;
}

bool CommandBuffer::isFull() const {
    return _commandQueue.size() >= Protocol::MAX_BUFFERED_COMMANDS;
}

void CommandBuffer::processCommands(int timeUnits) {
    std::queue<PendingCommand> remainingCommands;
    
    while (!_commandQueue.empty()) {
        PendingCommand cmd = _commandQueue.front();
        _commandQueue.pop();
        
        cmd.timeRemaining -= timeUnits;
        
        if (cmd.timeRemaining <= 0) {
            executeCommand(cmd);
        } else {
            remainingCommands.push(cmd);
        }
    }
    
    _commandQueue = remainingCommands;
}

void CommandBuffer::executeCommand(const PendingCommand& cmd) {
    std::string cmdName = Protocol::parseCommandName(cmd.command);
    std::string args = Protocol::parseCommandArguments(cmd.command);
    std::string response = Protocol::RESP_KO;
    
    if (cmdName == Protocol::CMD_FORWARD) {
        // TODO: Implement movement logic
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_RIGHT) {
        cmd.player->turnRight();
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_LEFT) {
        cmd.player->turnLeft();
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_LOOK) {
        response = formatLookResponse(cmd.player);
    } else if (cmdName == Protocol::CMD_INVENTORY) {
        response = formatInventoryResponse(cmd.player);
    } else if (cmdName == Protocol::CMD_BROADCAST) {
        // TODO: Implement broadcast logic
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_CONNECT_NBR) {
        // TODO: Get available slots from connection manager
        response = "0";
    } else if (cmdName == Protocol::CMD_FORK) {
        // TODO: Implement fork logic
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_EJECT) {
        // TODO: Implement eject logic
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_TAKE) {
        // TODO: Implement take logic
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_SET) {
        // TODO: Implement set logic
        response = Protocol::RESP_OK;
    } else if (cmdName == Protocol::CMD_INCANTATION) {
        // TODO: Implement incantation logic
        response = Protocol::RESP_OK;
    }
    
    // TODO: Send response back to client
    std::cout << "Command executed: " << cmd.command << " -> " << response << std::endl;
}

std::string CommandBuffer::formatInventoryResponse(Player* player) {
    std::string response = "[food " + std::to_string(player->getFood());
    
    for (int i = static_cast<int>(ResourceType::LINEMATE); i <= static_cast<int>(ResourceType::THYSTAME); ++i) {
        ResourceType type = static_cast<ResourceType>(i);
        int count = player->getResourceCount(type);
        response += ", " + resourceTypeToString(type) + " " + std::to_string(count);
    }
    
    response += "]";
    return response;
}

std::string CommandBuffer::formatLookResponse(Player* player) {
    // TODO: Implement actual vision logic
    // For now, return empty tile
    return "[player]";
}

void CommandBuffer::clear() {
    while (!_commandQueue.empty()) {
        _commandQueue.pop();
    }
}

} // namespace clientapi 