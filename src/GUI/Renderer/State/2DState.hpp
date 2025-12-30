/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** 2DState.hpp
*/

#pragma once

#include "IState.hpp"
#include "../UILayer/UILayer.hpp"
#include "../Renderer/State/Player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include "../../../ClientAPI/ClientAPIWrapper.hpp"
#include "../../../ClientAPI/Model/GameState.hpp"

typedef struct Tile_s {
    int player = 0;
    int food = 0;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;
} Tile_t;

class TwoDState : public IState
{
public:
    TwoDState(sf::RenderWindow &window, clientapi::ClientAPIWrapper &api);

    void onEnter(StateMachine &machine) override;
    void onExit(StateMachine &machine) override;
    void update(StateMachine &machine, float deltaTime) override;
    void render(StateMachine &machine) override;
    void handleEvent(StateMachine &machine, const sf::Event &event) override;

    // Game-specific methods
    void setMapSize(int height, int width);
    void setPlayerCount(int count);
    void setFoodCount(int count);
    void spawnFood();
    void spawnPlayer();
    void movePlayers();
    void displayMap();
    void initiateDrawables();
    void displayTile(int posx, int posy, float cubeHeight, float cubeWidth, int index);
    void displayPlayers(int posx, int posy, float cubeHeight, float cubeWidth, int index);
    void displayResources(int posx, int posy, float cubeHeight, float cubeWidth, int index);
    
    // Helper functions for enhanced player display
    sf::Color getTeamColor(const std::string &teamName);
    void drawOrientationIndicator(float centerX, float centerY, float radius, int orientation);
    void drawDetailedPlayerInfo(const clientapi::Player &player, float x, float y, float cubeWidth);
    
    // Helper functions for resource display
    sf::Color getResourceColor(int resourceType);
    void drawResourcePill(float x, float y, float radius, sf::Color color, int quantity);
    void drawResourceLegend();

private:
    sf::RenderWindow &window;
    UILayer uiLayer;
    clientapi::ClientAPIWrapper &m_api;

    sf::Clock m_clock;
    sf::View m_view;

    // Textures and sprites
    sf::Texture waterTexture;
    sf::Sprite waterSprite;
    sf::IntRect waterRect;
    sf::Texture islandTexture;
    sf::Sprite islandSprite;
    sf::Music bgMusic;
    sf::Font font;

    // Game data
    int mapWidth = 0;
    int mapHeight = 0;
    std::vector<Player> players;
    std::unique_ptr<clientapi::Map> gameMap;
    std::vector<Tile_t> tiles;
    int playerCount = 0;
    int foodCount = 0;
    bool isDisplayTile = false;
    bool displayPlayerInfo = false;
    bool displayResourceLegend = false;
    bool autoHideLegend = true;
    float legendTimer = 0.0f;

    // API callbacks and data
    void setupAPICallbacks();
    void updateFromAPI();
    void processTileData(int x, int y, const std::vector<int> &resources);
    std::array<int, clientapi::ResourceType::COUNT> m_totalResources = {0};
    bool m_hasMapData = false;
    int m_timeUnit = 0;
    
    // Team color management - random colors per instance but consistent per team
    std::unordered_map<std::string, sf::Color> m_teamColors;
    std::vector<sf::Color> m_availableColors;
    size_t m_nextColorIndex = 0;
    void initializeTeamColors();
};
