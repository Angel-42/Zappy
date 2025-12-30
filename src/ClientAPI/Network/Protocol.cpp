/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Protocol - Zappy GUI protocol constants and utilities
*/

#include "Protocol.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace clientapi {

// Server -> Client messages
const std::string Protocol::MSG_MAP_SIZE = "msz";
const std::string Protocol::MSG_TILE_CONTENT = "bct";
const std::string Protocol::MSG_MAP_CONTENT = "mct";
const std::string Protocol::MSG_TEAM_NAMES = "tna";
const std::string Protocol::MSG_PLAYER_NEW = "pnw";
const std::string Protocol::MSG_PLAYER_POSITION = "ppo";
const std::string Protocol::MSG_PLAYER_LEVEL = "plv";
const std::string Protocol::MSG_PLAYER_INVENTORY = "pin";
const std::string Protocol::MSG_PLAYER_EXPULSION = "pex";
const std::string Protocol::MSG_PLAYER_BROADCAST = "pbc";
const std::string Protocol::MSG_INCANTATION_START = "pic";
const std::string Protocol::MSG_INCANTATION_END = "pie";
const std::string Protocol::MSG_EGG_LAYING = "pfk";
const std::string Protocol::MSG_RESOURCE_DROP = "pdr";
const std::string Protocol::MSG_RESOURCE_COLLECT = "pgt";
const std::string Protocol::MSG_PLAYER_DEATH = "pdi";
const std::string Protocol::MSG_EGG_LAID = "enw";
const std::string Protocol::MSG_EGG_HATCHING = "ebo";
const std::string Protocol::MSG_EGG_DEATH = "edi";
const std::string Protocol::MSG_TIME_UNIT = "sgt";
const std::string Protocol::MSG_GAME_END = "seg";
const std::string Protocol::MSG_SERVER_MESSAGE = "smg";
const std::string Protocol::MSG_UNKNOWN_COMMAND = "suc";
const std::string Protocol::MSG_BAD_PARAMETER = "sbp";

// Client -> Server commands
const std::string Protocol::CMD_MAP_SIZE = "msz";
const std::string Protocol::CMD_TILE_CONTENT = "bct";
const std::string Protocol::CMD_MAP_CONTENT = "mct";
const std::string Protocol::CMD_TEAM_NAMES = "tna";
const std::string Protocol::CMD_PLAYER_POSITION = "ppo";
const std::string Protocol::CMD_PLAYER_LEVEL = "plv";
const std::string Protocol::CMD_PLAYER_INVENTORY = "pin";
const std::string Protocol::CMD_TIME_UNIT = "sgt";
const std::string Protocol::CMD_TIME_UNIT_MOD = "sst";

const std::string Protocol::WELCOME_MESSAGE = "WELCOME";

const std::string Protocol::RESP_OK = "ok";
const std::string Protocol::RESP_KO = "ko";
const std::string Protocol::CMD_FORWARD = "Forward";
const std::string Protocol::CMD_RIGHT = "Right";
const std::string Protocol::CMD_LEFT = "Left";
const std::string Protocol::CMD_LOOK = "Look";
const std::string Protocol::CMD_INVENTORY = "Inventory";
const std::string Protocol::CMD_BROADCAST = "Broadcast";
const std::string Protocol::CMD_CONNECT_NBR = "Connect_nbr";
const std::string Protocol::CMD_FORK = "Fork";
const std::string Protocol::CMD_EJECT = "Eject";
const std::string Protocol::CMD_TAKE = "Take";
const std::string Protocol::CMD_SET = "Set";
const std::string Protocol::CMD_INCANTATION = "Incantation";

// Helper functions
bool Protocol::parseInt(const std::string& str, int& value) {
    try {
        value = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

bool Protocol::parseVector(const std::string& str, std::vector<int>& values) {
    std::istringstream iss(str);
    std::string token;
    values.clear();
    while (std::getline(iss, token, ' ')) {
        int value;
        if (!parseInt(token, value)) {
            return false;
        }
        values.push_back(value);
    }
    return true;
}

std::vector<std::string> Protocol::split(const std::string& str, char delim) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, delim)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

/**
 * @brief Parse a map size message
 * 
 * Parses a message in the format "msz X Y" where X and Y are the map dimensions.
 * 
 * @param message The message to parse
 * @param width Output parameter for map width
 * @param height Output parameter for map height
 * @return true if parsing was successful
 */
bool Protocol::parseMapSize(const std::string& message, int& width, int& height) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 3 || parts[0] != MSG_MAP_SIZE) {
        return false;
    }
    return parseInt(parts[1], width) && parseInt(parts[2], height);
}

/**
 * @brief Parse a tile content message
 * 
 * Parses a message in the format "bct X Y q0 q1 q2 q3 q4 q5 q6" where X,Y are coordinates
 * and q0-q6 are resource quantities.
 * 
 * @param message The message to parse
 * @param x Output parameter for tile x coordinate
 * @param y Output parameter for tile y coordinate
 * @param resources Output parameter for resource quantities
 * @return true if parsing was successful
 */
bool Protocol::parseTileContent(const std::string& message, int& x, int& y, std::vector<int>& resources) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 10 || parts[0] != MSG_TILE_CONTENT) {
        return false;
    }

    if (!parseInt(parts[1], x) || !parseInt(parts[2], y)) {
        return false;
    }

    resources.clear();
    for (size_t i = 3; i < parts.size(); ++i) {
        int value;
        if (!parseInt(parts[i], value)) {
            return false;
        }
        resources.push_back(value);
    }
    return true;
}

/**
 * @brief Parse a new player message
 * 
 * Parses a message in the format "pnw #n X Y O L N" where:
 * - #n is the player ID
 * - X,Y are coordinates
 * - O is orientation
 * - L is level
 * - N is team name
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @param x Output parameter for x coordinate
 * @param y Output parameter for y coordinate
 * @param orientation Output parameter for orientation
 * @param level Output parameter for level
 * @param team Output parameter for team name
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerNew(const std::string& message, int& id, int& x, int& y, int& orientation, int& level, std::string& team) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 7 || parts[0] != MSG_PLAYER_NEW) {
        return false;
    }

    if (!parseInt(parts[1], id) || // Remove '#' prefix
        !parseInt(parts[2], x) ||
        !parseInt(parts[3], y) ||
        !parseInt(parts[4], orientation) ||
        !parseInt(parts[5], level)) {
        std::cout << "[DEBUG] parsePlayerNew: Failed to parse player new message" << std::endl;
        return false;
    }

    team = parts[6];
    std::cout << "[DEBUG] parsePlayerNew: Player new message parsed successfully" << std::endl;
    return true;
}

/**
 * @brief Parse a player position message
 * 
 * Parses a message in the format "ppo #n X Y O" where:
 * - #n is the player ID
 * - X,Y are coordinates
 * - O is orientation
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @param x Output parameter for x coordinate
 * @param y Output parameter for y coordinate
 * @param orientation Output parameter for orientation
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerPosition(const std::string& message, int& id, int& x, int& y, int& orientation) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 5 || parts[0] != MSG_PLAYER_POSITION) {
        return false;
    }

    return parseInt(parts[1], id) && // Remove '#' prefix
           parseInt(parts[2], x) &&
           parseInt(parts[3], y) &&
           parseInt(parts[4], orientation);
}

/**
 * @brief Parse a player level message
 * 
 * Parses a message in the format "plv #n L" where:
 * - #n is the player ID
 * - L is the level
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @param level Output parameter for level
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerLevel(const std::string& message, int& id, int& level) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 3 || parts[0] != MSG_PLAYER_LEVEL) {
        return false;
    }

    return parseInt(parts[1].substr(1), id) && // Remove '#' prefix
           parseInt(parts[2], level);
}

/**
 * @brief Parse a player inventory message
 * 
 * Parses a message in the format "pin #n X Y q0 q1 q2 q3 q4 q5 q6" where:
 * - #n is the player ID
 * - X,Y are coordinates
 * - q0-q6 are inventory quantities
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @param x Output parameter for x coordinate
 * @param y Output parameter for y coordinate
 * @param inventory Output parameter for inventory quantities
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerInventory(const std::string& message, int& id, int& x, int& y, std::vector<int>& inventory) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 11 || parts[0] != MSG_PLAYER_INVENTORY) {
        return false;
    }

    if (!parseInt(parts[1].substr(1), id) || // Remove '#' prefix
        !parseInt(parts[2], x) ||
        !parseInt(parts[3], y)) {
        return false;
    }

    inventory.clear();
    for (size_t i = 4; i < parts.size(); ++i) {
        int value;
        if (!parseInt(parts[i], value)) {
            return false;
        }
        inventory.push_back(value);
    }
    return true;
}

/**
 * @brief Parse a player expulsion message
 * 
 * Parses a message in the format "pex #n" where #n is the player ID.
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerExpulsion(const std::string& message, int& id) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 2 || parts[0] != MSG_PLAYER_EXPULSION) {
        return false;
    }

    return parseInt(parts[1].substr(1), id); // Remove '#' prefix
}

/**
 * @brief Parse a player broadcast message
 * 
 * Parses a message in the format "pbc #n M" where:
 * - #n is the player ID
 * - M is the broadcast message
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @param outMessage Output parameter for broadcast message
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerBroadcast(const std::string& message, int& id, std::string& outMessage) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() < 3 || parts[0] != MSG_PLAYER_BROADCAST) {
        return false;
    }

    if (!parseInt(parts[1].substr(1), id)) { // Remove '#' prefix
        return false;
    }

    outMessage = parts[2];
    for (size_t i = 3; i < parts.size(); ++i) {
        outMessage += " " + parts[i];
    }
    return true;
}

/**
 * @brief Parse an incantation start message
 * 
 * Parses a message in the format "pic X Y L #n #n ..." where:
 * - X,Y are coordinates
 * - L is the level
 * - #n are participating player IDs
 * 
 * @param message The message to parse
 * @param x Output parameter for x coordinate
 * @param y Output parameter for y coordinate
 * @param level Output parameter for level
 * @param players Output parameter for participating player IDs
 * @return true if parsing was successful
 */
bool Protocol::parseIncantationStart(const std::string& message, int& x, int& y, int& level, std::vector<int>& players) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() < 5 || parts[0] != MSG_INCANTATION_START) {
        return false;
    }

    if (!parseInt(parts[1], x) || !parseInt(parts[2], y) || !parseInt(parts[3], level)) {
        return false;
    }

    players.clear();
    for (size_t i = 4; i < parts.size(); ++i) {
        int id;
        if (!parseInt(parts[i], id)) {
            return false;
        }
        players.push_back(id);
    }
    return true;
}

/**
 * @brief Parse an incantation end message
 * 
 * Parses a message in the format "pie X Y R" where:
 * - X,Y are coordinates
 * - R is the result (0 for failure, 1 for success)
 * 
 * @param message The message to parse
 * @param x Output parameter for x coordinate
 * @param y Output parameter for y coordinate
 * @param result Output parameter for result
 * @return true if parsing was successful
 */
bool Protocol::parseIncantationEnd(const std::string& message, int& x, int& y, int& result) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 4 || parts[0] != MSG_INCANTATION_END) {
        return false;
    }

    return parseInt(parts[1], x) && parseInteger(parts[2], y) && parseInteger(parts[3], result);
}

/**
 * @brief Parse a resource drop message
 * 
 * Parses a message in the format "pdr #n i" where:
 * - #n is the player ID
 * - i is the resource ID
 * 
 * @param message The message to parse
 * @param playerId Output parameter for player ID
 * @param resourceId Output parameter for resource ID
 * @return true if parsing was successful
 */
bool Protocol::parseResourceDrop(const std::string& message, int& playerId, int& resourceId) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 3 || parts[0] != MSG_RESOURCE_DROP) {
        return false;
    }

    return parseInt(parts[1], playerId) && parseInt(parts[2], resourceId);
}

/**
 * @brief Parse a resource collect message
 * 
 * Parses a message in the format "pgt #n i" where:
 * - #n is the player ID
 * - i is the resource ID
 * 
 * @param message The message to parse
 * @param playerId Output parameter for player ID
 * @param resourceId Output parameter for resource ID
 * @return true if parsing was successful
 */
bool Protocol::parseResourceCollect(const std::string& message, int& playerId, int& resourceId) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 3 || parts[0] != MSG_RESOURCE_COLLECT) {
        return false;
    }

    return parseInt(parts[1], playerId) && parseInt(parts[2], resourceId);
}

/**
 * @brief Parse a player death message
 * 
 * Parses a message in the format "pdi #n" where #n is the player ID.
 * 
 * @param message The message to parse
 * @param id Output parameter for player ID
 * @return true if parsing was successful
 */
bool Protocol::parsePlayerDeath(const std::string& message, int& id) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 2 || parts[0] != MSG_PLAYER_DEATH) {
        return false;
    }

    return parseInt(parts[1], id);
}

/**
 * @brief Parse an egg laid message
 * 
 * Parses a message in the format "enw #e #n X Y" where:
 * - #e is the egg ID
 * - #n is the player ID
 * - X,Y are coordinates
 * 
 * @param message The message to parse
 * @param eggId Output parameter for egg ID
 * @param playerId Output parameter for player ID
 * @param x Output parameter for x coordinate
 * @param y Output parameter for y coordinate
 * @return true if parsing was successful
 */
bool Protocol::parseEggLaid(const std::string& message, int& eggId, int& playerId, int& x, int& y) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 5 || parts[0] != MSG_EGG_LAID) {
        return false;
    }

    return parseInt(parts[1].substr(1), eggId) && // Remove '#' prefix
           parseInt(parts[2].substr(1), playerId) && // Remove '#' prefix
           parseInt(parts[3], x) &&
           parseInt(parts[4], y);
}

/**
 * @brief Parse an egg hatching message
 * 
 * Parses a message in the format "ebo #e" where #e is the egg ID.
 * 
 * @param message The message to parse
 * @param eggId Output parameter for egg ID
 * @return true if parsing was successful
 */
bool Protocol::parseEggHatching(const std::string& message, int& eggId) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 2 || parts[0] != MSG_EGG_HATCHING) {
        return false;
    }

    return parseInt(parts[1].substr(1), eggId); // Remove '#' prefix
}

/**
 * @brief Parse an egg death message
 * 
 * Parses a message in the format "edi #e" where #e is the egg ID.
 * 
 * @param message The message to parse
 * @param eggId Output parameter for egg ID
 * @return true if parsing was successful
 */
bool Protocol::parseEggDeath(const std::string& message, int& eggId) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 2 || parts[0] != MSG_EGG_DEATH) {
        return false;
    }

    return parseInt(parts[1].substr(1), eggId); // Remove '#' prefix
}

/**
 * @brief Parse a time unit message
 * 
 * Parses a message in the format "sgt T" where T is the time unit.
 * 
 * @param message The message to parse
 * @param timeUnit Output parameter for time unit
 * @return true if parsing was successful
 */
bool Protocol::parseTimeUnit(const std::string& message, int& timeUnit) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 2 || parts[0] != MSG_TIME_UNIT) {
        return false;
    }

    return parseInt(parts[1], timeUnit);
}

/**
 * @brief Parse a game end message
 * 
 * Parses a message in the format "seg N" where N is the winning team name.
 * 
 * @param message The message to parse
 * @param winningTeam Output parameter for winning team name
 * @return true if parsing was successful
 */
bool Protocol::parseGameEnd(const std::string& message, std::string& winningTeam) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() != 2 || parts[0] != MSG_GAME_END) {
        return false;
    }

    winningTeam = parts[1];
    return true;
}

/**
 * @brief Parse a server message
 * 
 * Parses a message in the format "smg M" where M is the server message.
 * 
 * @param message The message to parse
 * @param outMessage Output parameter for server message
 * @return true if parsing was successful
 */
bool Protocol::parseServerMessage(const std::string& message, std::string& outMessage) {
    std::vector<std::string> parts = split(message, ' ');
    if (parts.size() < 2 || parts[0] != MSG_SERVER_MESSAGE) {
        return false;
    }

    outMessage = parts[1];
    for (size_t i = 2; i < parts.size(); ++i) {
        outMessage += " " + parts[i];
    }
    return true;
}

bool Protocol::parseInteger(const std::string& str, int& value) {
    try {
        value = std::stoi(str);
        return true;
    } catch (...) {
        return false;
    }
}

// Message formatting functions
/**
 * @brief Format a map size request
 * @return The formatted request
 */
std::string Protocol::formatMapSizeRequest() {
    return CMD_MAP_SIZE + "\n";
}

/**
 * @brief Format a tile content request
 * @param x The x coordinate
 * @param y The y coordinate
 * @return The formatted request
 */
std::string Protocol::formatTileContentRequest(int x, int y) {
    return CMD_TILE_CONTENT + " " + std::to_string(x) + " " + std::to_string(y) + "\n";
}

/**
 * @brief Format a map content request
 * @return The formatted request
 */
std::string Protocol::formatMapContentRequest() {
    return CMD_MAP_CONTENT + "\n";
}

/**
 * @brief Format a team names request
 * @return The formatted request
 */
std::string Protocol::formatTeamNamesRequest() {
    return CMD_TEAM_NAMES + "\n";
}

/**
 * @brief Format a player position request
 * @param playerId The player ID
 * @return The formatted request
 */
std::string Protocol::formatPlayerPositionRequest(int playerId) {
    return CMD_PLAYER_POSITION + " #" + std::to_string(playerId) + "\n";
}

/**
 * @brief Format a player level request
 * @param playerId The player ID
 * @return The formatted request
 */
std::string Protocol::formatPlayerLevelRequest(int playerId) {
    return CMD_PLAYER_LEVEL + " #" + std::to_string(playerId) + "\n";
}

/**
 * @brief Format a player inventory request
 * @param playerId The player ID
 * @return The formatted request
 */
std::string Protocol::formatPlayerInventoryRequest(int playerId) {
    return CMD_PLAYER_INVENTORY + " #" + std::to_string(playerId) + "\n";
}

/**
 * @brief Format a time unit request
 * @return The formatted request
 */
std::string Protocol::formatTimeUnitRequest() {
    return CMD_TIME_UNIT + "\n";
}

/**
 * @brief Format a time unit modification request
 * @param timeUnit The new time unit value
 * @return The formatted request
 */
std::string Protocol::formatTimeUnitModification(int timeUnit) {
    return CMD_TIME_UNIT_MOD + " " + std::to_string(timeUnit) + "\n";
}

bool Protocol::isValidCommand(const std::string& command) {
    static const std::vector<std::string> valid = {
        CMD_FORWARD, CMD_RIGHT, CMD_LEFT, CMD_LOOK, CMD_INVENTORY, CMD_BROADCAST,
        CMD_CONNECT_NBR, CMD_FORK, CMD_EJECT, CMD_TAKE, CMD_SET, CMD_INCANTATION
    };
    std::string name = parseCommandName(command);
    return std::find(valid.begin(), valid.end(), name) != valid.end();
}

int Protocol::getCommandTimeLimit(const std::string& command) {
    // Dummy implementation: all commands take 7 units
    return 7;
}

std::string Protocol::parseCommandName(const std::string& command) {
    auto pos = command.find(' ');
    return (pos == std::string::npos) ? command : command.substr(0, pos);
}

std::string Protocol::parseCommandArguments(const std::string& command) {
    auto pos = command.find(' ');
    return (pos == std::string::npos) ? "" : command.substr(pos + 1);
}

} // namespace clientapi 