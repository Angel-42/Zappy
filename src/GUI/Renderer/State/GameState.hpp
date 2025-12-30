/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameState.hpp
*/

#pragma once

#include "IState.hpp"
#include "../Scene.hpp"
#include "../UILayer/UILayer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include "../../../ClientAPI/ClientAPIWrapper.hpp"
#include "../../../ClientAPI/Model/GameState.hpp"
#include "../../SFMLUtils/Text/Text.hpp"

class GameState : public IState
{
public:
    GameState(sf::RenderWindow &window, clientapi::ClientAPIWrapper &api);

    void onEnter(StateMachine &machine) override;
    void onExit(StateMachine &machine) override;
    void update(StateMachine &machine, float deltaTime) override;
    void render(StateMachine &machine) override;
    void handleEvent(StateMachine &machine, const sf::Event &event) override;

private:
    sf::RenderWindow &window;
    std::unique_ptr<Scene> scene;
    UILayer uiLayer;
    clientapi::ClientAPIWrapper &m_api;

    sf::Font m_font;
    std::shared_ptr<sf::Text> m_statsTextPtr;

    void setupAPICallbacks();
    void updateResourceList();
    void updatePlayers();

    void processTileData(int x, int y, const std::vector<int> &resources);
    std::array<int, clientapi::ResourceType::COUNT> m_totalResources = {0};
    bool m_hasMapData = false;
    int m_timeUnit = 0;
};
