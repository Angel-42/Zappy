/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Game.hpp
*/

#pragma once
#include "Event/EventDispatcher.hpp"
#include "Event/Event.hpp"
#include <memory>
#include <SFML/Window/Event.hpp>

class Game
{
public:
    Game() : dispatcher(std::make_shared<EventDispatcher>()) {}

    std::shared_ptr<EventDispatcher> getDispatcher() { return dispatcher; }

    void handleSfmlEvent(const sf::Event &sfEvent)
    {
        if (sfEvent.type == sf::Event::Closed)
        {
            Event event{Event::WindowClosed};
            dispatcher->dispatch(event);
        }
        // Map other SFML events as needed
    }

private:
    std::shared_ptr<EventDispatcher> dispatcher;
};
