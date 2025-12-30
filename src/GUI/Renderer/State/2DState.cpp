/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** 2DState.cpp
*/

/*
** ENHANCED PLAYER DISPLAY WITH CIRCLES AND RESOURCE VISUALIZATION
** 
** This implementation displays players as colored circles and resources as colored pills,
** using the same principle as map size retrieval (getting data from API GameState).
** 
** Key features:
** - Colored circles for each player based on team (randomized colors per instance)
** - Circle outline thickness indicates player level
** - Orientation indicator (small triangle showing direction)
** - Detailed player information display (toggle with 'I' key)
** - Resource visualization as colored pills on tiles
** - Resource legend positioned on the left side (auto-shows for 8s, toggle with 'R')
** - Full utilization of Player getters: getId(), getTeam(), getLevel(), 
**   getPosition(), getOrientation(), getInventory(), getFood(), getResourceCount()
** 
** Resource Colors:
** - Food: Brown, Linemate: Silver, Deraumere: Gold, Sibur: Purple
** - Mendiane: Cyan, Phiras: Magenta, Thystame: Lime Green
** 
** Controls:
** - T: Toggle tile display
** - I: Toggle detailed player information
** - R: Toggle resource legend (disables auto-hide)
** - Space: Player information debug
** - +/-: Zoom in/out
** - Arrow keys: Pan view
*/

#include "2DState.hpp"
#include "StateMachine.hpp"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <functional>
#include <GL/glew.h>
#include <GL/gl.h>
#include <random>
#include <algorithm>

TwoDState::TwoDState(sf::RenderWindow &window, clientapi::ClientAPIWrapper &api) 
    : window(window), m_api(api)
{
    std::cout << "> Initializing 2D State" << std::endl;
    initializeTeamColors();
}

void TwoDState::onEnter(StateMachine &)
{
    std::cout << "> Entering 2D State" << std::endl;
    
    // Disable OpenGL states that might be enabled by 3D mode
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // Reset SFML states
    window.resetGLStates();
    window.pushGLStates();
    
    m_view.setSize(800, 600);
    m_view.setCenter(400, 300);
    window.setView(m_view);
    window.setSize(sf::Vector2u(1280, 720));
    if (!waterTexture.loadFromFile("src/GUI/Renderer/State/2Dassets/waterCartoon.png")) {
        std::cerr << "Failed to load water texture" << std::endl;
    }
    waterSprite.setTexture(waterTexture);
    waterRect = sf::IntRect(0, 0, 800, 600);
    waterSprite.setTextureRect(waterRect);

    if (!islandTexture.loadFromFile("src/GUI/Renderer/State/2Dassets/map.png")) {
        std::cerr << "Failed to load island texture" << std::endl;
    }
    islandSprite.setTexture(islandTexture);
    islandSprite.setPosition(-100, -100);
    islandSprite.setScale(1.30f, 1.30f);

    if (!bgMusic.openFromFile("src/GUI/Renderer/State/2Dassets/music.ogg")) {
        std::cerr << "Failed to load background music" << std::endl;
    }
    bgMusic.setLoop(true);
    bgMusic.setVolume(20);
    // bgMusic.play();
    
    // Check if map exists before accessing it
    const auto* map = m_api.getGameState().getMap();
    if (map) {
        this->setMapSize(map->getHeight(), map->getWidth());
        std::cout << "Map size set to: "
                  << map->getHeight() << "x"
                  << map->getWidth() << std::endl;
    } else {
        std::cout << "Map not yet initialized, using default size" << std::endl;
        this->setMapSize(10, 10); // Default size until map is received
    }

    this->font.loadFromFile("arial.ttf");
    
    // Setup API callbacks
    setupAPICallbacks();
    
    // Show resource legend by default for 8 seconds
    displayResourceLegend = true;
    autoHideLegend = true;
    legendTimer = 0.0f;
}

void TwoDState::onExit(StateMachine &)
{
    std::cout << "> Exiting 2D State" << std::endl;
    bgMusic.stop();
    uiLayer.clear();
    
    // Clean up SFML states
    window.popGLStates();
}

void TwoDState::update(StateMachine &, float deltaTime)
{
    (void)deltaTime;
    waterRect.left += 1;
    if (waterRect.left == 1000) {
        waterRect.left = 0;
    }
    waterSprite.setTextureRect(waterRect);
    
    for (auto &player : players) {
        player.setScale(0.23f / mapHeight, 0.17f / mapHeight);
    }

    static float timeSinceLastUpdate = 0;
    timeSinceLastUpdate += deltaTime;

    if (timeSinceLastUpdate > 1.0f) {
        m_api.sendMapContentRequest();
        updateFromAPI();
        timeSinceLastUpdate = 0;
    }
    
    // Handle auto-hide legend timer
    if (displayResourceLegend && autoHideLegend) {
        legendTimer += deltaTime;
        if (legendTimer > 8.0f) { // Hide after 8 seconds
            displayResourceLegend = false;
            autoHideLegend = false;
        }
    }
}

void TwoDState::render(StateMachine &)
{
    // Ensure we're in 2D mode
    window.resetGLStates();
    window.pushGLStates();
    
    window.clear();

    window.draw(waterSprite);
    window.draw(islandSprite);
    displayMap();
    
    // Save current view and switch to default view for UI elements
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());
    
    drawResourceLegend();
    
    // Restore previous view
    window.setView(currentView);
    
    uiLayer.draw(window);
    
    window.popGLStates();
}

void TwoDState::handleEvent(StateMachine &, const sf::Event &event)
{
    uiLayer.handleEvent(event);
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::T) {
            isDisplayTile = !isDisplayTile;
        }
        if (event.key.code == sf::Keyboard::I) {
            displayPlayerInfo = !displayPlayerInfo;
        }
        if (event.key.code == sf::Keyboard::R) {
            displayResourceLegend = !displayResourceLegend;
            autoHideLegend = false; // Disable auto-hide when user manually toggles
        }
        if (event.key.code == sf::Keyboard::Space) {
            movePlayers();
        }
        if (event.key.code == sf::Keyboard::Add) {
            m_view.zoom(0.9f);
            window.setView(m_view);
        }
        if (event.key.code == sf::Keyboard::Subtract) {
            m_view.zoom(1.1f);
            if (m_view.getSize().x > 800 || m_view.getSize().y > 600) {
                m_view.setSize(800, 600);
                m_view.setCenter(400, 300);
            }
            window.setView(m_view);
        }
        if (event.key.code == sf::Keyboard::Left) {
            m_view.move(-10, 0);
            window.setView(m_view);
            if (m_view.getCenter().x - m_view.getSize().x / 2 < 0) {
                m_view.setCenter(m_view.getSize().x / 2, m_view.getCenter().y);
                window.setView(m_view);
            }
        }
        if (event.key.code == sf::Keyboard::Right) {
            m_view.move(10, 0);
            window.setView(m_view);
            if (m_view.getCenter().x + m_view.getSize().x / 2 > 800) {
                m_view.setCenter(800 - m_view.getSize().x / 2, m_view.getCenter().y);
                window.setView(m_view);
            }
        }
        if (event.key.code == sf::Keyboard::Up) {
            m_view.move(0, -10);
            window.setView(m_view);
            if (m_view.getCenter().y - m_view.getSize().y / 2 < 0) {
                m_view.setCenter(m_view.getCenter().x, m_view.getSize().y / 2);
                window.setView(m_view);
            }
        }
        if (event.key.code == sf::Keyboard::Down) {
            m_view.move(0, 10);
            window.setView(m_view);
            if (m_view.getCenter().y + m_view.getSize().y / 2 > 600) {
                m_view.setCenter(m_view.getCenter().x, 600 - m_view.getSize().y / 2);
                window.setView(m_view);
            }
        }
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            std::cout << "Mouse position: "
                      << event.mouseButton.x << ", "
                      << event.mouseButton.y << std::endl;
        }
    }
}

void TwoDState::setMapSize(int height, int width)
{
    this->mapHeight = height;
    this->mapWidth = width;
    this->tiles.resize(mapHeight * mapWidth);
    for (int i = 0; i < mapHeight * mapWidth; ++i) {
        tiles[i] = Tile_t();
    }
}

void TwoDState::setPlayerCount(int count)
{
    playerCount = count;
    players.resize(count);
}

void TwoDState::spawnPlayer()
{
    players.push_back(Player());
}

void TwoDState::movePlayers()
{
    std::cout << "nb of players: " << players.size() << std::endl;
}

void TwoDState::displayTile(int posx, int posy, float cubeHeight, float cubeWidth, int index)
{
    sf::RectangleShape tile(sf::Vector2f(cubeWidth, cubeHeight));
    tile.setOutlineThickness(1);
    tile.setOutlineColor(sf::Color::Black);
    tile.setPosition(posx, posy);
    if (index % 2 == 0) {
        tile.setFillColor(sf::Color(100, 100, 100, 128));
    } else {
        tile.setFillColor(sf::Color(150, 150, 150, 128));
    }
    window.draw(tile);
}

void TwoDState::displayPlayers(int posx, int posy, float cubeHeight, float cubeWidth, int index)
{
    // Get current tile coordinates
    int tileX = index % mapWidth;
    int tileY = index / mapWidth;
    
    // Get all players from the GameState API
    const auto &apiPlayers = m_api.getGameState().getPlayers();
    
    std::cout << "PlayerSize: " << apiPlayers.size() << std::endl;
    float playerRadius = std::min(cubeWidth, cubeHeight) * 0.3f;
    float spacing = std::min(cubeWidth, cubeHeight) * 0.15f;
    int playersOnTile = 0;
    
    // Draw all players on this tile using API data
    for (const auto &[id, player] : apiPlayers) {
        const auto &pos = player.getPosition();
        if (pos.x == tileX && pos.y == tileY) {
            // Calculate position for this player (multiple players can be on same tile)
            float offsetX = (playersOnTile % 2) * spacing;
            float offsetY = (playersOnTile / 2) * spacing;
            
            // Draw player circle
            sf::CircleShape playerCircle(playerRadius);
            playerCircle.setPosition(posx + cubeWidth * 0.2f + offsetX, posy + cubeHeight * 0.2f + offsetY);
            
            // Color based on team (generate consistent colors)
            sf::Color teamColor = getTeamColor(player.getTeam());
            playerCircle.setFillColor(teamColor);
            
            // Add outline based on level
            playerCircle.setOutlineThickness(player.getLevel());
            playerCircle.setOutlineColor(sf::Color::White);
            
            window.draw(playerCircle);
            
            // Draw orientation indicator
            drawOrientationIndicator(posx + cubeWidth * 0.2f + offsetX + playerRadius, 
                                   posy + cubeHeight * 0.2f + offsetY + playerRadius, 
                                   playerRadius, player.getOrientation());
            
            // Draw detailed player info if enabled
            if (displayPlayerInfo) {
                drawDetailedPlayerInfo(player, posx + offsetX, posy + offsetY + cubeHeight * 0.7f, cubeWidth);
            }
            
            playersOnTile++;
        }
    }
}

void TwoDState::displayResources(int posx, int posy, float cubeHeight, float cubeWidth, int index)
{
    // Always display resources as pills (not dependent on isDisplayTile)
    
    // Calculate pill size and spacing based on tile size
    float pillRadius = std::min(cubeWidth, cubeHeight) * 0.08f;
    float spacing = pillRadius * 2.5f;
    
    // Starting position for pills (top edge of the tile)
    float startX = posx + pillRadius + 2;
    float startY = posy + pillRadius + 2;
    
    // Array of resource quantities in order
    int resources[] = {
        tiles[index].food,
        tiles[index].linemate,
        tiles[index].deraumere,
        tiles[index].sibur,
        tiles[index].mendiane,
        tiles[index].phiras,
        tiles[index].thystame
    };
    
    // Draw pills for each resource type that has quantity > 0
    int pillCount = 0;
    for (int i = 0; i < 7; i++) {
        if (resources[i] > 0) {
            // Calculate position (arrange in a line along the top)
            float pillX = startX + (pillCount % 4) * spacing;
            float pillY = startY + (pillCount / 4) * spacing;
            
            // Make sure pills stay within the tile bounds
            if (pillX + pillRadius > posx + cubeWidth) {
                pillX = posx + cubeWidth - pillRadius - 2;
            }
            if (pillY + pillRadius > posy + cubeHeight) {
                pillY = posy + cubeHeight - pillRadius - 2;
            }
            
            drawResourcePill(pillX, pillY, pillRadius, getResourceColor(i), resources[i]);
            pillCount++;
        }
    }
}

void TwoDState::displayMap()
{
    for (int i = 0; i < mapHeight; ++i) {
        for (int j = 0; j < mapWidth; ++j) {
            int index = i * mapWidth + j;
            float cubeHeight = (640 - 215) / this->mapHeight + (mapHeight / 10.0f * 0.02f);
            float cubeWidth = (640 - 215) / this->mapWidth + (mapWidth / 10.0f * 0.02f);
            int posx = (215) + j * cubeWidth;
            int posy = (45) + i * cubeHeight;

            if (isDisplayTile)
                displayTile(posx, posy, cubeHeight, cubeWidth, index);
            displayPlayers(posx, posy, cubeHeight, cubeWidth, index);
            displayResources(posx, posy, cubeHeight, cubeWidth, index);
        }
    }
}

void TwoDState::initiateDrawables()
{
    int indexPlayer = 0;

    for (int i = this->players.size(); i < playerCount; ++i) {
        players.emplace_back();
    }
    tiles.resize(mapHeight * mapWidth);
    for (int i = 0; i < mapHeight * mapWidth; ++i) {
        tiles[i] = Tile_t();
    }

    for (int i = 0; i < playerCount; ++i) {
        int x = rand() % mapWidth;
        int y = rand() % mapHeight;
        while (tiles[y * mapWidth + x].player > 0) {
            x = rand() % mapWidth;
            y = rand() % mapHeight;
        }
        tiles[y * mapWidth + x].player = 1;
        indexPlayer++;
    }
}

void TwoDState::setupAPICallbacks()
{
    m_api.setMapSizeCallback([this](int width, int height) {
        std::cout << "Map size received: " << width << "x" << height << std::endl;
        
        // Create the map in the GameState
        clientapi::Map map(width, height);
        m_api.getGameState().setMap(map);
        
        this->setMapSize(width, height);
        std::fill(m_totalResources.begin(), m_totalResources.end(), 0);
        m_hasMapData = false;
        m_api.sendMapContentRequest();
    });

    m_api.setTileContentCallback([this](int x, int y, const std::vector<int> &resources) {
        if (x == 0 && y == 0) {
            std::fill(m_totalResources.begin(), m_totalResources.end(), 0);
        }
        processTileData(x, y, resources);
    });

    m_api.setTeamNamesCallback([this](const std::vector<std::string> &teams) {
        std::cout << "Team names received: " << teams.size() << " teams" << std::endl;
    });

    m_api.setPlayerNewCallback([this](int id, int x, int y, int orientation, int level, const std::string &team) {
        std::cout << "New player #" << id << " from team " << team << " at position (" << x << "," << y << ")" << std::endl;
        
        // Create player with constructor that sets id, team, and position
        clientapi::Player player(id, team, {x, y});
        player.setOrientation(orientation);
        player.setLevel(level);
        
        m_api.getGameState().addOrUpdatePlayer(player);
        
        // Update tiles structure
        if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
            tiles[y * mapWidth + x].player += 1;
        }
    });

    m_api.setPlayerPositionCallback([this](int id, int x, int y, int orientation) {
        std::cout << "Player #" << id << " moved to position (" << x << "," << y << ")" << std::endl;
        
        // Update player position in GameState
        auto* player = m_api.getGameState().getPlayer(id);
        if (player) {
            player->setPosition({x, y});
            player->setOrientation(orientation);
        }
        
        // Clear old player counts and recalculate
        for (auto &tile : tiles) {
            tile.player = 0;
        }
        
        // Recalculate all player positions
        const auto& players = m_api.getGameState().getPlayers();
        for (const auto &[playerId, playerObj] : players) {
            const auto &pos = playerObj.getPosition();
            if (pos.x >= 0 && pos.x < mapWidth && pos.y >= 0 && pos.y < mapHeight) {
                tiles[pos.y * mapWidth + pos.x].player += 1;
            }
        }
    });

    m_api.setPlayerLevelCallback([this](int id, int level) {
        std::cout << "Player #" << id << " is now level " << level << std::endl;
    });

    m_api.setEggLaidCallback([this](int eggId, int playerId, int x, int y) {
        std::cout << "Egg #" << eggId << " laid by player #" << playerId << " at position (" << x << "," << y << ")" << std::endl;
    });

    m_api.setEggHatchingCallback([this](int eggId) {
        std::cout << "Egg #" << eggId << " is hatching" << std::endl;
    });

    m_api.setEggDeathCallback([this](int eggId) {
        std::cout << "Egg #" << eggId << " has died" << std::endl;
    });

    m_api.setPlayerDeathCallback([this](int id) {
        std::cout << "Player #" << id << " has died - removing from GUI" << std::endl;
        
        // Remove player from the GameState
        m_api.getGameState().removePlayer(id);
        
        // Recalculate player count on all tiles
        for (auto &tile : tiles) {
            tile.player = 0;
        }
        
        // Count remaining players on each tile
        const auto& players = m_api.getGameState().getPlayers();
        for (const auto& [playerId, player] : players) {
            auto pos = player.getPosition();
            int x = pos.x;
            int y = pos.y;
            if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
                tiles[y * mapWidth + x].player += 1;
            }
        }
    });

    m_api.setTimeUnitCallback([this](int timeUnit) {
        std::cout << "Time unit updated to: " << timeUnit << std::endl;
        m_timeUnit = timeUnit;
    });

    m_api.sendTimeUnitRequest();

    std::cout << "Requesting map data from server..." << std::endl;
    m_api.sendMapSizeRequest();
    std::cout << "Map data request sent" << std::endl;
}

void TwoDState::processTileData(int x, int y, const std::vector<int> &resources)
{
    m_hasMapData = true;

    // Update tiles structure with received data
    if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
        int index = y * mapWidth + x;
        if (index < static_cast<int>(tiles.size())) {
            // Clear previous resources for this tile
            tiles[index].food = 0;
            tiles[index].linemate = 0;
            tiles[index].deraumere = 0;
            tiles[index].sibur = 0;
            tiles[index].mendiane = 0;
            tiles[index].phiras = 0;
            tiles[index].thystame = 0;

            // Update with new data
            if (resources.size() > clientapi::FOOD)
                tiles[index].food = resources[clientapi::FOOD];
            if (resources.size() > clientapi::LINEMATE)
                tiles[index].linemate = resources[clientapi::LINEMATE];
            if (resources.size() > clientapi::DERAUMERE)
                tiles[index].deraumere = resources[clientapi::DERAUMERE];
            if (resources.size() > clientapi::SIBUR)
                tiles[index].sibur = resources[clientapi::SIBUR];
            if (resources.size() > clientapi::MENDIANE)
                tiles[index].mendiane = resources[clientapi::MENDIANE];
            if (resources.size() > clientapi::PHIRAS)
                tiles[index].phiras = resources[clientapi::PHIRAS];
            if (resources.size() > clientapi::THYSTAME)
                tiles[index].thystame = resources[clientapi::THYSTAME];
        }
    }

    // Update total resources for statistics
    for (int i = 0; i < std::min(resources.size(), (size_t)clientapi::ResourceType::COUNT); i++) {
        m_totalResources[i] += resources[i];
    }
}

void TwoDState::updateFromAPI()
{
    // Update players from API data using all getters
    const auto &apiPlayers = m_api.getGameState().getPlayers();
    
    // Clear all player counts in tiles
    for (auto &tile : tiles) {
        tile.player = 0;
    }
    
    // Update player positions based on API data and log detailed info
    for (const auto &[id, player] : apiPlayers) {
        const auto &pos = player.getPosition();
        
        // Log comprehensive player information using all getters
        std::cout << "Player #" << player.getId() 
                  << " (Team: " << player.getTeam() 
                  << ", Level: " << player.getLevel()
                  << ", Pos: (" << pos.x << "," << pos.y << ")"
                  << ", Orientation: " << player.getOrientation() << ")" << std::endl;
        
        // Log inventory using resource getters
        std::cout << "  Inventory - Food: " << player.getFood()
                  << ", Linemate: " << player.getResourceCount(clientapi::LINEMATE)
                  << ", Deraumere: " << player.getResourceCount(clientapi::DERAUMERE)
                  << ", Sibur: " << player.getResourceCount(clientapi::SIBUR)
                  << ", Mendiane: " << player.getResourceCount(clientapi::MENDIANE)
                  << ", Phiras: " << player.getResourceCount(clientapi::PHIRAS)
                  << ", Thystame: " << player.getResourceCount(clientapi::THYSTAME) << std::endl;
        
        // Update tile with player presence
        if (pos.x >= 0 && pos.x < mapWidth && pos.y >= 0 && pos.y < mapHeight) {
            tiles[pos.y * mapWidth + pos.x].player += 1;
        }
    }
    
    std::cout << "Updated from API: " << apiPlayers.size() << " players" << std::endl;
}

// Helper functions for enhanced player display
void TwoDState::initializeTeamColors()
{
    // Create a pool of distinct colors
    std::vector<sf::Color> baseColors = {
        sf::Color::Red,
        sf::Color::Blue, 
        sf::Color::Green,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
        sf::Color(255, 165, 0), // Orange
        sf::Color(128, 0, 128), // Purple
        sf::Color(255, 192, 203), // Pink
        sf::Color(0, 128, 0), // Dark Green
        sf::Color(255, 215, 0), // Gold
        sf::Color(139, 69, 19), // Brown
        sf::Color(128, 128, 128), // Gray
        sf::Color(255, 20, 147), // Deep Pink
        sf::Color(75, 0, 130), // Indigo
        sf::Color(50, 205, 50) // Lime Green
    };
    
    // Shuffle the colors to make them random for each instance
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(baseColors.begin(), baseColors.end(), g);
    
    m_availableColors = baseColors;
    m_nextColorIndex = 0;
}

sf::Color TwoDState::getTeamColor(const std::string &teamName)
{
    // Check if we already have a color for this team
    auto it = m_teamColors.find(teamName);
    if (it != m_teamColors.end()) {
        return it->second;
    }
    
    // Assign a new color to this team
    if (m_nextColorIndex < m_availableColors.size()) {
        sf::Color teamColor = m_availableColors[m_nextColorIndex];
        m_teamColors[teamName] = teamColor;
        m_nextColorIndex++;
        
        std::cout << "Assigned color to team '" << teamName << "' (color index: " << (m_nextColorIndex - 1) << ")" << std::endl;
        return teamColor;
    } else {
        // If we run out of predefined colors, generate a random color
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        
        sf::Color randomColor(dis(gen), dis(gen), dis(gen));
        m_teamColors[teamName] = randomColor;
        
        std::cout << "Generated random color for team '" << teamName << "'" << std::endl;
        return randomColor;
    }
}

void TwoDState::drawOrientationIndicator(float centerX, float centerY, float radius, int orientation)
{
    // Draw a small triangle indicating direction
    sf::CircleShape triangle(radius * 0.3f, 3);
    triangle.setFillColor(sf::Color::Black);
    triangle.setOrigin(radius * 0.3f, radius * 0.3f);
    triangle.setPosition(centerX, centerY);
    
    // Rotate based on orientation (1=N, 2=E, 3=S, 4=W)
    float angle = 0;
    switch (orientation) {
        case 1: angle = -90; break; // North
        case 2: angle = 0; break;   // East
        case 3: angle = 90; break;  // South
        case 4: angle = 180; break; // West
    }
    triangle.setRotation(angle);
    
    window.draw(triangle);
}

void TwoDState::drawDetailedPlayerInfo(const clientapi::Player &player, float x, float y, float cubeWidth)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(std::max(6.0f, cubeWidth * 0.08f));
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);
    
    // Build comprehensive player info using all getters
    std::stringstream info;
    info << "ID:" << player.getId() << "\n";
    info << "Team:" << player.getTeam() << "\n";
    info << "Lvl:" << player.getLevel() << "\n";
    info << "Pos:(" << player.getPosition().x << "," << player.getPosition().y << ")\n";
    info << "Dir:" << player.getOrientation() << "\n";
    
    // Display inventory using all resource getters
    const auto &inventory = player.getInventory();
    info << "Inventory:\n";
    info << "F:" << player.getFood() << " ";
    info << "L:" << player.getResourceCount(clientapi::LINEMATE) << "\n";
    info << "D:" << player.getResourceCount(clientapi::DERAUMERE) << " ";
    info << "S:" << player.getResourceCount(clientapi::SIBUR) << "\n";
    info << "M:" << player.getResourceCount(clientapi::MENDIANE) << " ";
    info << "P:" << player.getResourceCount(clientapi::PHIRAS) << "\n";
    info << "T:" << player.getResourceCount(clientapi::THYSTAME);
    
    text.setString(info.str());
    text.setPosition(x, y);
    window.draw(text);
}

// Helper functions for resource display
sf::Color TwoDState::getResourceColor(int resourceType)
{
    // Define specific colors for each resource type
    switch (resourceType) {
        case 0: // Food
            return sf::Color(139, 69, 19); // Brown
        case 1: // Linemate
            return sf::Color(192, 192, 192); // Silver
        case 2: // Deraumere
            return sf::Color(255, 215, 0); // Gold
        case 3: // Sibur
            return sf::Color(128, 0, 128); // Purple
        case 4: // Mendiane
            return sf::Color(0, 255, 255); // Cyan
        case 5: // Phiras
            return sf::Color(255, 0, 255); // Magenta
        case 6: // Thystame
            return sf::Color(50, 205, 50); // Lime Green
        default:
            return sf::Color::White;
    }
}

void TwoDState::drawResourcePill(float x, float y, float radius, sf::Color color, int quantity)
{
    if (quantity <= 0) return;
    
    // Draw the main pill (circle)
    sf::CircleShape pill(radius);
    pill.setPosition(x - radius, y - radius);
    pill.setFillColor(color);
    pill.setOutlineThickness(1);
    pill.setOutlineColor(sf::Color::Black);
    
    window.draw(pill);
    
    // Draw quantity text if > 1
    if (quantity > 1) {
        sf::Text quantityText;
        quantityText.setFont(font);
        quantityText.setCharacterSize(std::max(4.0f, radius * 0.8f));
        quantityText.setFillColor(sf::Color::White);
        quantityText.setOutlineColor(sf::Color::Black);
        quantityText.setOutlineThickness(0.5f);
        quantityText.setString(std::to_string(quantity));
        
        // Center the text on the pill
        sf::FloatRect textBounds = quantityText.getLocalBounds();
        quantityText.setPosition(x - textBounds.width / 2, y - textBounds.height / 2 - 1);
        
        window.draw(quantityText);
    }
}

void TwoDState::drawResourceLegend()
{
    if (!displayResourceLegend) return;
    
    // Get window size to position legend on the left side
    sf::Vector2u windowSize = window.getSize();
    float legendWidth = 180;
    float legendHeight = 160;
    
    // Position on the left side
    float legendX = 15;
    float legendY = 50;
    
    // Background for the legend
    sf::RectangleShape legendBg(sf::Vector2f(legendWidth, legendHeight));
    legendBg.setPosition(legendX, legendY);
    legendBg.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black
    legendBg.setOutlineThickness(2);
    legendBg.setOutlineColor(sf::Color::White);
    window.draw(legendBg);
    
    // Title
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(14);
    title.setFillColor(sf::Color::White);
    title.setString("Resource Legend");
    title.setPosition(legendX + 10, legendY + 5);
    window.draw(title);
    
    // Resource names and their corresponding colors
    std::vector<std::string> resourceNames = {
        "Food", "Linemate", "Deraumere", "Sibur", 
        "Mendiane", "Phiras", "Thystame"
    };
    
    float startY = legendY + 30;
    float lineHeight = 16;
    float pillRadius = 6;
    
    for (int i = 0; i < 7; i++) {
        float y = startY + i * lineHeight;
        
        // Draw colored pill
        sf::CircleShape pill(pillRadius);
        pill.setPosition(legendX + 15 - pillRadius, y - pillRadius);
        pill.setFillColor(getResourceColor(i));
        pill.setOutlineThickness(1);
        pill.setOutlineColor(sf::Color::Black);
        window.draw(pill);
        
        // Draw resource name
        sf::Text resourceText;
        resourceText.setFont(font);
        resourceText.setCharacterSize(12);
        resourceText.setFillColor(sf::Color::White);
        resourceText.setString(resourceNames[i]);
        resourceText.setPosition(legendX + 30, y - 8);
        window.draw(resourceText);
    }
    
    // Instructions
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(10);
    instructions.setFillColor(sf::Color(200, 200, 200));
    
    std::string instructionText = "Press R to toggle legend";
    if (autoHideLegend) {
        int timeLeft = (int)(8.0f - legendTimer);
        if (timeLeft > 0) {
            instructionText += " (auto-hide in " + std::to_string(timeLeft) + "s)";
        }
    }
    
    instructions.setString(instructionText);
    instructions.setPosition(legendX + 10, legendY + legendHeight - 20);
    window.draw(instructions);
}
