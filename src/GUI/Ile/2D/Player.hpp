/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Player {
    public:
        Player();
        ~Player();
        void draw(sf::RenderWindow &window, float x, float y);
        void setScale(float x, float y);

    private:
        sf::Sprite m_sprite; // Sprite for the player
        sf::Texture m_texture; // Texture for the player sprite
};

#endif /* !PLAYER_HPP_ */
