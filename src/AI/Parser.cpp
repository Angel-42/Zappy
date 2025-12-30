/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Parser.cpp
*/

#include "Parser.hpp"
#include <regex>
#include <sstream>
#include <iostream>

Parser::Parser(WorldState &s) : st(s) {}

void Parser::setWorldSize(int width, int height)
{
    worldWidth = width;
    worldHeight = height;
    std::cout << "[DEBUG] World size set to: " << width << "x" << height << std::endl;
}

void Parser::ingest(const std::string &line)
{
    std::cout << "[DEBUG] Parser: Ingesting line: '" << line << "'" << std::endl;
    std::smatch m;

    if (std::regex_match(line, m, std::regex(R"((\d+)\s+(\d+))")))
    {
        if (worldWidth == 0 && worldHeight == 0)
        {
            std::cout << "[DEBUG] Parser: Setting world size from line: " << line << std::endl;
            setWorldSize(std::stoi(m[1]), std::stoi(m[2]));
            return;
        }
    }

    if (std::regex_match(line, m, std::regex(R"(Current level:\s*(\d+))")))
    {
        int newLevel = std::stoi(m[1]);
        std::cout << "[DEBUG] Parser: Level updated from " << st.level << " to " << newLevel << std::endl;
        st.level = newLevel;
    }
    else if (std::regex_match(line, m, std::regex(R"(\{([^}]*)\})")))
    {
        std::cout << "[DEBUG] Parser: Parsing inventory response: " << m[1] << std::endl;
        parseInventory(m[1]);
    }
    else if (std::regex_match(line, m, std::regex(R"(\[([^\]]*)\])")))
    {
        // Check if this is an inventory response (contains numbers) or look response (contains resource names)
        std::string payload = m[1];
        if (payload.find("food") != std::string::npos && 
            (payload.find(" 0") != std::string::npos || payload.find(" 1") != std::string::npos || 
             payload.find(" 2") != std::string::npos || payload.find(" 3") != std::string::npos ||
             payload.find(" 4") != std::string::npos || payload.find(" 5") != std::string::npos ||
             payload.find(" 6") != std::string::npos || payload.find(" 7") != std::string::npos ||
             payload.find(" 8") != std::string::npos || payload.find(" 9") != std::string::npos))
        {
            // This is an inventory response (contains resource names with counts)
            std::cout << "[DEBUG] Parser: Parsing inventory response: " << payload << std::endl;
            parseInventory(payload);
        }
        else
        {
            // This is a look response (contains resource names without counts)
            std::cout << "[DEBUG] Parser: Parsing look response: " << payload << std::endl;
            parseLook(payload);
        }
    }
    else if (line == "Elevation underway")
    {
        // Elevation started successfully
        std::cout << "[DEBUG] Parser: Elevation started!" << std::endl;
    }
    else if (std::regex_match(line, m, std::regex(R"(message\s+(\d+),\s*(.+))")))
    {
        // Broadcast message received
        int direction = std::stoi(m[1]);
        std::string message = m[2];
        std::cout << "[DEBUG] Parser: Broadcast from direction " << direction << ": " << message << std::endl;
    }
    else
    {
        std::cout << "[DEBUG] Parser: Unrecognized line format: " << line << std::endl;
    }
}

void Parser::parseInventory(const std::string &payload)
{
    std::cout << "[DEBUG] parseInventory called with payload: " << payload << std::endl;
    st.inventory.clear();
    std::istringstream ss(payload);
    std::string pair;
    while (std::getline(ss, pair, ','))
    {
        std::istringstream p(pair);
        std::string name;
        int cnt;
        if (p >> name >> cnt)
        {
            st.inventory[name] = cnt;
            if (name == "food")
                st.food = cnt;
            else if (name == "linemate")
                st.linemate = cnt;
            else if (name == "deraumere")
                st.deraumere = cnt;
            else if (name == "sibur")
                st.sibur = cnt;
            else if (name == "mendiane")
                st.mendiane = cnt;
            else if (name == "phiras")
                st.phiras = cnt;
            else if (name == "thystame")
                st.thystame = cnt;
        }
    }
    std::cout << "[DEBUG] parseInventory: updated WorldState - food:" << st.food 
              << " linemate:" << st.linemate << " deraumere:" << st.deraumere 
              << " sibur:" << st.sibur << " mendiane:" << st.mendiane 
              << " phiras:" << st.phiras << " thystame:" << st.thystame << std::endl;
}

void Parser::parseLook(const std::string &payload)
{
    std::cout << "[DEBUG] Parsing Look response: " << payload << std::endl;

    st.vision.clear();
    std::istringstream ss(payload);
    std::string tile;
    int tileIndex = 0;

    while (std::getline(ss, tile, ','))
    {
        std::istringstream ts(tile);
        std::string tok;
        std::vector<std::string> tokens;
        while (ts >> tok)
            tokens.push_back(tok);
        st.vision.push_back(tokens);

        if (tileIndex == 0)
        {
            auto [normX, normY] = normalizePosition(st.posX, st.posY);
            st.mapMemory.markVisited(normX, normY);
            st.mapMemory.setTile(normX, normY, tokens);
        }
        else
        {
            auto [dx, dy] = calculateTileOffset(tileIndex, st.orientation);
            int tileX = st.posX + dx;
            int tileY = st.posY + dy;
            auto [normX, normY] = normalizePosition(tileX, tileY);
            st.mapMemory.setTile(normX, normY, tokens);
        }
        tileIndex++;
    }
    std::cout << "[DEBUG] Updated map memory with " << tileIndex << " tiles" << std::endl;
}

std::pair<int, int> Parser::normalizePosition(int x, int y) const
{
    if (worldWidth > 0 && worldHeight > 0)
    {
        x = ((x % worldWidth) + worldWidth) % worldWidth;
        y = ((y % worldHeight) + worldHeight) % worldHeight;
    }
    return {x, y};
}

void Parser::onCommandAck(const std::string &cmd)
{
    std::cout << "[DEBUG] Parser: Command acknowledged: " << cmd << std::endl;
    
    if (cmd == "Forward")
    {
        int oldX = st.posX, oldY = st.posY;
        switch (st.orientation)
        {
        case Orientation::NORTH:
            st.posY -= 1;
            break;
        case Orientation::EAST:
            st.posX += 1;
            break;
        case Orientation::SOUTH:
            st.posY += 1;
            break;
        case Orientation::WEST:
            st.posX -= 1;
            break;
        }

        auto [normX, normY] = normalizePosition(st.posX, st.posY);
        st.posX = normX;
        st.posY = normY;
        std::cout << "[DEBUG] Parser: Position updated from (" << oldX << "," << oldY << ") to (" << st.posX << "," << st.posY << ")" << std::endl;
    }
    else if (cmd == "Left")
    {
        Orientation oldOrientation = st.orientation;
        st.orientation = static_cast<Orientation>((static_cast<int>(st.orientation) + 3) % 4);
        std::cout << "[DEBUG] Parser: Orientation changed from " << static_cast<int>(oldOrientation) << " to " << static_cast<int>(st.orientation) << " (Left)" << std::endl;
    }
    else if (cmd == "Right")
    {
        Orientation oldOrientation = st.orientation;
        st.orientation = static_cast<Orientation>((static_cast<int>(st.orientation) + 1) % 4);
        std::cout << "[DEBUG] Parser: Orientation changed from " << static_cast<int>(oldOrientation) << " to " << static_cast<int>(st.orientation) << " (Right)" << std::endl;
    }
    else
    {
        std::cout << "[DEBUG] Parser: Command acknowledged but no state change: " << cmd << std::endl;
    }
}

std::pair<int, int> Parser::calculateTileOffset(int tileIndex, Orientation orientation) const
{
    std::pair<int, int> offset = {0, 0};

    switch (tileIndex)
    {
    case 0: // Current tile
        offset = {0, 0};
        break;
    case 1: // Front tile
        switch (orientation)
        {
        case Orientation::NORTH:
            offset = {0, -1};
            break;
        case Orientation::EAST:
            offset = {1, 0};
            break;
        case Orientation::SOUTH:
            offset = {0, 1};
            break;
        case Orientation::WEST:
            offset = {-1, 0};
            break;
        }
        break;
    case 2: // Front-left tile
        switch (orientation)
        {
        case Orientation::NORTH:
            offset = {-1, -1};
            break;
        case Orientation::EAST:
            offset = {1, -1};
            break;
        case Orientation::SOUTH:
            offset = {1, 1};
            break;
        case Orientation::WEST:
            offset = {-1, 1};
            break;
        }
        break;
    case 3: // Front-right tile
        switch (orientation)
        {
        case Orientation::NORTH:
            offset = {1, -1};
            break;
        case Orientation::EAST:
            offset = {1, 1};
            break;
        case Orientation::SOUTH:
            offset = {-1, 1};
            break;
        case Orientation::WEST:
            offset = {-1, -1};
            break;
        }
        break;
    default:
        offset = {0, 0};
        break;
    }

    return offset;
}