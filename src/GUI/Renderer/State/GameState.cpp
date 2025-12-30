/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameState.cpp
*/

#include "GameState.hpp"
#include "StateMachine.hpp"
#include <sstream>

GameState::GameState(sf::RenderWindow &window, clientapi::ClientAPIWrapper &api)
    : window(window), scene(nullptr), m_api(clientapi::ClientAPIWrapper::getInstance())
{
    // Load font for stats display
    if (!m_font.loadFromFile("arial.ttf"))
    {
        std::cerr << "Failed to load font" << std::endl;
    }

    auto textElement = std::make_shared<sf::Text>();
    textElement->setFont(m_font);
    textElement->setString("Loading...");
    textElement->setCharacterSize(20);
    textElement->setFillColor(sf::Color::White);
    textElement->setPosition(sf::Vector2f(10, 10));
    uiLayer.addElement(textElement);
    m_statsTextPtr = textElement;
}

void GameState::onEnter(StateMachine &)
{
    std::cout << "> Entering Game State" << std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    scene = std::make_unique<Scene>(Scene::RenderMode::Mode3D, window);
    scene->initialize();

    setupAPICallbacks();
}

void GameState::onExit(StateMachine &)
{
    std::cout << "> Exiting Game State" << std::endl;
    if (scene)
        scene->cleanup();
    scene.reset();

    glUseProgram(0);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void GameState::update(StateMachine &, float deltaTime)
{
    if (scene)
        scene->update(deltaTime);

    static float timeSinceLastUpdate = 0;
    timeSinceLastUpdate += deltaTime;

    if (timeSinceLastUpdate > 1.0f)
    {
        m_api.sendMapContentRequest();
        updateResourceList();
        updatePlayers();

        timeSinceLastUpdate = 0;
    }
}

void GameState::render(StateMachine &)
{
    if (scene)
        scene->render(window);
    window.resetGLStates();
    window.pushGLStates();
    uiLayer.draw(window);
    window.popGLStates();
}

void GameState::handleEvent(StateMachine &, const sf::Event &event)
{
}

void GameState::updatePlayers()
{
    auto *renderer3d = dynamic_cast<Renderer3D *>(scene->getRenderer());
    if (!renderer3d)
        return;

    renderer3d->clearPlayers();

    const auto &players = m_api.getGameState().getPlayers();
    for (const auto &[id, player] : players)
    {
        float x = static_cast<float>(player.getPosition().x);
        float z = static_cast<float>(player.getPosition().y);
        float y = 0.5f;

        renderer3d->addPlayer(
            id,
            glm::vec3(x, y, z),
            player.getTeam(),
            player.getLevel());
    }
}

void GameState::processTileData(int x, int y, const std::vector<int> &resources)
{
    m_hasMapData = true;

    for (int i = 0; i < std::min(resources.size(), (size_t)clientapi::ResourceType::COUNT); i++)
    {
        m_totalResources[i] += resources[i];
    }
}

void GameState::setupAPICallbacks()
{
    m_api.setMapSizeCallback([this](int width, int height)
                             {
        std::cout << "Map size received: " << width << "x" << height << std::endl;
        std::fill(m_totalResources.begin(), m_totalResources.end(), 0);
        m_hasMapData = false;
        m_api.sendMapContentRequest(); });

    m_api.setTileContentCallback([this](int x, int y, const std::vector<int> &resources)
                                 {
    if (x == 0 && y == 0) {
        std::fill(m_totalResources.begin(), m_totalResources.end(), 0);
    }
    
    processTileData(x, y, resources); });

    m_api.setTeamNamesCallback([this](const std::vector<std::string> &teams)
                               { std::cout << "Team names received: " << teams.size() << " teams" << std::endl; });

    m_api.setPlayerNewCallback([this](int id, int x, int y, int orientation, int level, const std::string &team)
                               { std::cout << "New player #" << id << " from team " << team << " at position (" << x << "," << y << ")" << std::endl; });

    m_api.setPlayerPositionCallback([this](int id, int x, int y, int orientation)
                                    { std::cout << "Player #" << id << " moved to position (" << x << "," << y << ")" << std::endl; });

    m_api.setPlayerLevelCallback([this](int id, int level)
                                 { std::cout << "Player #" << id << " is now level " << level << std::endl; });

    m_api.setEggLaidCallback([this](int eggId, int playerId, int x, int y)
                             { std::cout << "Egg #" << eggId << " laid by player #" << playerId << " at position (" << x << "," << y << ")" << std::endl; });

    m_api.setEggHatchingCallback([this](int eggId)
                                 { std::cout << "Egg #" << eggId << " is hatching" << std::endl; });

    m_api.setEggDeathCallback([this](int eggId)
                              { std::cout << "Egg #" << eggId << " has died" << std::endl; });

    m_api.setPlayerDeathCallback([this](int id)
                                { 
                                    std::cout << "Player #" << id << " has died" << std::endl;
                                    m_api.getGameState().removePlayer(id);
                                });

    m_api.setTimeUnitCallback([this](int timeUnit)
                              {
                                  std::cout << "Time unit updated to: " << timeUnit << std::endl;
                                  m_timeUnit = timeUnit; });

    m_api.sendTimeUnitRequest();

    std::cout << "Requesting map data from server..." << std::endl;
    m_api.sendMapSizeRequest();
    std::cout << "Map data request sent" << std::endl;
}

void GameState::updateResourceList()
{
    std::stringstream ss;

    ss << "Time Unit: " << m_timeUnit << " t/s\n\n";

    ss << "Resources on map:\n\n";

    if (m_hasMapData)
    {
        ss << "Food: " << m_totalResources[clientapi::FOOD] << "\n";
        ss << "Linemate: " << m_totalResources[clientapi::LINEMATE] << "\n";
        ss << "Deraumere: " << m_totalResources[clientapi::DERAUMERE] << "\n";
        ss << "Sibur: " << m_totalResources[clientapi::SIBUR] << "\n";
        ss << "Mendiane: " << m_totalResources[clientapi::MENDIANE] << "\n";
        ss << "Phiras: " << m_totalResources[clientapi::PHIRAS] << "\n";
        ss << "Thystame: " << m_totalResources[clientapi::THYSTAME] << "\n";
    }
    else
    {
        ss << "Map data not available yet";
    }

    const auto &players = m_api.getGameState().getPlayers();
    ss << "\nPlayers on map: " << players.size() << "\n";
    for (const auto &[id, player] : players)
    {
        ss << "Player #" << id << " (Lvl " << player.getLevel() << ") - Team: " << player.getTeam() << "\n";
        ss << "  Position: " << player.getPosition().x << "," << player.getPosition().y << "\n";

        const auto &inventory = player.getInventory();
        ss << "  Inventory: Food:" << inventory[clientapi::FOOD]
           << " L:" << inventory[clientapi::LINEMATE]
           << " D:" << inventory[clientapi::DERAUMERE]
           << " S:" << inventory[clientapi::SIBUR]
           << " M:" << inventory[clientapi::MENDIANE]
           << " P:" << inventory[clientapi::PHIRAS]
           << " T:" << inventory[clientapi::THYSTAME] << "\n";
    }

    const auto &teams = m_api.getGameState().getTeams();
    ss << "\nTeams: " << teams.size() << "\n";
    for (const auto &[name, team] : teams)
    {
        ss << "Team " << name << ": " << team.getPlayers().size() << " players\n";
    }

    const auto &eggs = m_api.getGameState().getEggs();
    ss << "\nEggs on map: " << eggs.size() << "\n";
    for (const auto &[id, egg] : eggs)
    {
        ss << "Egg #" << id << " at position (" << egg.pos.x << "," << egg.pos.y << ")";
        if (egg.hatched)
            ss << " [HATCHED]";
        if (egg.dead)
            ss << " [DEAD]";
        ss << "\n";
    }

    if (m_statsTextPtr)
        m_statsTextPtr->setString(ss.str());
}
