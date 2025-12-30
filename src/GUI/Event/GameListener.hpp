/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameListener.hpp
*/

#pragma once
#include "Event/Event.hpp"
#include "Event/IEventListener.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>

class GameListener : public IEventListener
{
public:
    GameListener(sf::RenderWindow &window) : m_window(window) {}

    void onEvent(const Event &event) override
    {
        if (event.type == Event::WindowClosed)
        {
            std::cout << "Window closed event received!\n";
            m_window.close();
        }
    }

private:
    sf::RenderWindow &m_window;
};
