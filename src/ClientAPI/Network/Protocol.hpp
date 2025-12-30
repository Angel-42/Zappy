/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Protocol - Zappy GUI protocol constants and utilities
*/

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <string>
#include <vector>
#include <sstream>

namespace clientapi {

/**
 * @brief Protocol class for handling Zappy GUI protocol
 * 
 * This class provides constants and utilities for:
 * - Message formatting and parsing
 * - Protocol compliance checking
 * - Command validation
 * 
 * The protocol follows the Zappy GUI specification exactly, ensuring
 * consistent communication between the client and server.
 */
class Protocol {
    public:
        // Protocol message constants
        static const std::string MSG_MAP_SIZE;           ///< Map size message
        static const std::string MSG_TILE_CONTENT;       ///< Tile content message
        static const std::string MSG_MAP_CONTENT;        ///< Map content message
        static const std::string MSG_TEAM_NAMES;         ///< Team names message
        static const std::string MSG_PLAYER_NEW;         ///< New player message
        static const std::string MSG_PLAYER_POSITION;    ///< Player position message
        static const std::string MSG_PLAYER_LEVEL;       ///< Player level message
        static const std::string MSG_PLAYER_INVENTORY;   ///< Player inventory message
        static const std::string MSG_PLAYER_EXPULSION;   ///< Player expulsion message
        static const std::string MSG_PLAYER_BROADCAST;   ///< Player broadcast message
        static const std::string MSG_INCANTATION_START;  ///< Incantation start message
        static const std::string MSG_INCANTATION_END;    ///< Incantation end message
        static const std::string MSG_RESOURCE_DROP;      ///< Resource drop message
        static const std::string MSG_RESOURCE_COLLECT;   ///< Resource collect message
        static const std::string MSG_PLAYER_DEATH;       ///< Player death message
        static const std::string MSG_EGG_LAID;           ///< Egg laid message
        static const std::string MSG_EGG_HATCHING;       ///< Egg hatching message
        static const std::string MSG_EGG_DEATH;          ///< Egg death message
        static const std::string MSG_TIME_UNIT;          ///< Time unit message
        static const std::string MSG_GAME_END;           ///< Game end message
        static const std::string MSG_SERVER_MESSAGE;     ///< Server message
        static const std::string MSG_UNKNOWN_COMMAND;    ///< Unknown command message
        static const std::string MSG_BAD_PARAMETER;      ///< Bad parameter message
        static const std::string WELCOME_MESSAGE;
        static const std::string MSG_EGG_LAYING;
        // Protocol command constants
        static const std::string CMD_MAP_SIZE;           ///< Request map size
        static const std::string CMD_TILE_CONTENT;       ///< Request tile content
        static const std::string CMD_MAP_CONTENT;        ///< Request map content
        static const std::string CMD_TEAM_NAMES;         ///< Request team names
        static const std::string CMD_PLAYER_POSITION;    ///< Request player position
        static const std::string CMD_PLAYER_LEVEL;       ///< Request player level
        static const std::string CMD_PLAYER_INVENTORY;   ///< Request player inventory
        static const std::string CMD_TIME_UNIT;          ///< Request time unit
        static const std::string CMD_TIME_UNIT_MOD;      ///< Modify time unit
        static const std::string RESP_OK;
        static const std::string RESP_KO;
        static const std::string CMD_FORWARD;
        static const std::string CMD_RIGHT;
        static const std::string CMD_LEFT;
        static const std::string CMD_LOOK;
        static const std::string CMD_INVENTORY;
        static const std::string CMD_BROADCAST;
        static const std::string CMD_CONNECT_NBR;
        static const std::string CMD_FORK;
        static const std::string CMD_EJECT;
        static const std::string CMD_TAKE;
        static const std::string CMD_SET;
        static const std::string CMD_INCANTATION;
        static constexpr int MAX_BUFFERED_COMMANDS = 10;

        // Message parsing utilities
        /**
         * @brief Parse a map size message
         * @param message The message to parse
         * @param width Output parameter for map width
         * @param height Output parameter for map height
         * @return true if parsing was successful
         */
        static bool parseMapSize(const std::string& message, int& width, int& height);

        /**
         * @brief Parse a tile content message
         * @param message The message to parse
         * @param x Output parameter for tile x coordinate
         * @param y Output parameter for tile y coordinate
         * @param resources Output parameter for resource quantities
         * @return true if parsing was successful
         */
        static bool parseTileContent(const std::string& message, int& x, int& y, std::vector<int>& resources);

        /**
         * @brief Parse a new player message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @param x Output parameter for x coordinate
         * @param y Output parameter for y coordinate
         * @param orientation Output parameter for orientation
         * @param level Output parameter for level
         * @param team Output parameter for team name
         * @return true if parsing was successful
         */
        static bool parsePlayerNew(const std::string& message, int& id, int& x, int& y, int& orientation, int& level, std::string& team);

        /**
         * @brief Parse a player position message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @param x Output parameter for x coordinate
         * @param y Output parameter for y coordinate
         * @param orientation Output parameter for orientation
         * @return true if parsing was successful
         */
        static bool parsePlayerPosition(const std::string& message, int& id, int& x, int& y, int& orientation);

        /**
         * @brief Parse a player level message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @param level Output parameter for level
         * @return true if parsing was successful
         */
        static bool parsePlayerLevel(const std::string& message, int& id, int& level);

        /**
         * @brief Parse a player inventory message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @param x Output parameter for x coordinate
         * @param y Output parameter for y coordinate
         * @param inventory Output parameter for inventory quantities
         * @return true if parsing was successful
         */
        static bool parsePlayerInventory(const std::string& message, int& id, int& x, int& y, std::vector<int>& inventory);

        /**
         * @brief Parse a player expulsion message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @return true if parsing was successful
         */
        static bool parsePlayerExpulsion(const std::string& message, int& id);

        /**
         * @brief Parse a player broadcast message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @param outMessage Output parameter for broadcast message
         * @return true if parsing was successful
         */
        static bool parsePlayerBroadcast(const std::string& message, int& id, std::string& outMessage);

        /**
         * @brief Parse an incantation start message
         * @param message The message to parse
         * @param x Output parameter for x coordinate
         * @param y Output parameter for y coordinate
         * @param level Output parameter for level
         * @param players Output parameter for participating player IDs
         * @return true if parsing was successful
         */
        static bool parseIncantationStart(const std::string& message, int& x, int& y, int& level, std::vector<int>& players);

        /**
         * @brief Parse an incantation end message
         * @param message The message to parse
         * @param x Output parameter for x coordinate
         * @param y Output parameter for y coordinate
         * @param result Output parameter for result (0 for failure, 1 for success)
         * @return true if parsing was successful
         */
        static bool parseIncantationEnd(const std::string& message, int& x, int& y, int& result);

        /**
         * @brief Parse a resource drop message
         * @param message The message to parse
         * @param playerId Output parameter for player ID
         * @param resourceId Output parameter for resource ID
         * @return true if parsing was successful
         */
        static bool parseResourceDrop(const std::string& message, int& playerId, int& resourceId);

        /**
         * @brief Parse a resource collect message
         * @param message The message to parse
         * @param playerId Output parameter for player ID
         * @param resourceId Output parameter for resource ID
         * @return true if parsing was successful
         */
        static bool parseResourceCollect(const std::string& message, int& playerId, int& resourceId);

        /**
         * @brief Parse a player death message
         * @param message The message to parse
         * @param id Output parameter for player ID
         * @return true if parsing was successful
         */
        static bool parsePlayerDeath(const std::string& message, int& id);

        /**
         * @brief Parse an egg laid message
         * @param message The message to parse
         * @param eggId Output parameter for egg ID
         * @param playerId Output parameter for player ID
         * @param x Output parameter for x coordinate
         * @param y Output parameter for y coordinate
         * @return true if parsing was successful
         */
        static bool parseEggLaid(const std::string& message, int& eggId, int& playerId, int& x, int& y);

        /**
         * @brief Parse an egg hatching message
         * @param message The message to parse
         * @param eggId Output parameter for egg ID
         * @return true if parsing was successful
         */
        static bool parseEggHatching(const std::string& message, int& eggId);

        /**
         * @brief Parse an egg death message
         * @param message The message to parse
         * @param eggId Output parameter for egg ID
         * @return true if parsing was successful
         */
        static bool parseEggDeath(const std::string& message, int& eggId);

        /**
         * @brief Parse a time unit message
         * @param message The message to parse
         * @param timeUnit Output parameter for time unit
         * @return true if parsing was successful
         */
        static bool parseTimeUnit(const std::string& message, int& timeUnit);

        /**
         * @brief Parse a game end message
         * @param message The message to parse
         * @param winningTeam Output parameter for winning team name
         * @return true if parsing was successful
         */
        static bool parseGameEnd(const std::string& message, std::string& winningTeam);

        /**
         * @brief Parse a server message
         * @param message The message to parse
         * @param outMessage Output parameter for server message
         * @return true if parsing was successful
         */
        static bool parseServerMessage(const std::string& message, std::string& outMessage);

        // Message formatting utilities
        /**
         * @brief Format a map size request
         * @return The formatted request
         */
        static std::string formatMapSizeRequest();

        /**
         * @brief Format a tile content request
         * @param x The x coordinate
         * @param y The y coordinate
         * @return The formatted request
         */
        static std::string formatTileContentRequest(int x, int y);

        /**
         * @brief Format a map content request
         * @return The formatted request
         */
        static std::string formatMapContentRequest();

        /**
         * @brief Format a team names request
         * @return The formatted request
         */
        static std::string formatTeamNamesRequest();

        /**
         * @brief Format a player position request
         * @param playerId The player ID
         * @return The formatted request
         */
        static std::string formatPlayerPositionRequest(int playerId);

        /**
         * @brief Format a player level request
         * @param playerId The player ID
         * @return The formatted request
         */
        static std::string formatPlayerLevelRequest(int playerId);

        /**
         * @brief Format a player inventory request
         * @param playerId The player ID
         * @return The formatted request
         */
        static std::string formatPlayerInventoryRequest(int playerId);

        /**
         * @brief Format a time unit request
         * @return The formatted request
         */
        static std::string formatTimeUnitRequest();

        /**
         * @brief Format a time unit modification request
         * @param timeUnit The new time unit value
         * @return The formatted request
         */
        static std::string formatTimeUnitModification(int timeUnit);

        /**
         * @brief Split a string by a delimiter
         * @param str The string to split
         * @param delim The delimiter character
         * @return Vector of substrings
         */
        static std::vector<std::string> split(const std::string& str, char delim);

        static bool parseInt(const std::string& str, int& value);
        static bool parseVector(const std::string& str, std::vector<int>& values);

        static bool isValidCommand(const std::string& command);
        static int getCommandTimeLimit(const std::string& command);
        static std::string parseCommandName(const std::string& command);
        static std::string parseCommandArguments(const std::string& command);

    private:
        /**
         * @brief Parse an integer from a string
         * @param str The string to parse
         * @param value Output parameter for the parsed integer
         * @return true if parsing was successful
         */
        static bool parseInteger(const std::string& str, int& value);

        /**
         * @brief Parse a vector of integers from a string
         * @param str The string to parse
         * @param values Output parameter for the parsed integers
         * @return true if parsing was successful
         */
        static bool parseIntegerVector(const std::string& str, std::vector<int>& values);
};

} // namespace clientapi

#endif /* !PROTOCOL_HPP_ */ 