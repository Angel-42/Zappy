/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IState.hpp
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <memory>
#include <iostream>

class StateMachine;

class IState
{
public:
    virtual ~IState() = default;
    virtual void onEnter(StateMachine &machine) = 0; // Called when the state becomes active
    virtual void onExit(StateMachine &machine) = 0;  // Called when the state is about to be replaced
    virtual void update(StateMachine &machine, float deltaTime) = 0;
    virtual void render(StateMachine &machine) = 0;
    virtual void handleEvent(StateMachine &machine, const sf::Event &event) = 0;
};
