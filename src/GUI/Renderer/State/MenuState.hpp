/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** MenuState.hpp
*/

#pragma once

#include "IState.hpp"
#include "../UILayer/UILayer.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class MenuState : public IState
{
public:
    MenuState(sf::RenderWindow &window);

    void onEnter(StateMachine &machine) override;
    void onExit(StateMachine &machine) override;
    void update(StateMachine &machine, float deltaTime) override;
    void render(StateMachine &machine) override;
    void handleEvent(StateMachine &machine, const sf::Event &event) override;

private:
    sf::RenderWindow &window;
    UILayer uiLayer;

    sf::Font font;
    std::shared_ptr<sf::Texture> logoTexture;
};
