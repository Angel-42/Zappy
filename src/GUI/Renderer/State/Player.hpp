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
        void setPosition(int x, int y) { posX = x; posY = y; }
        int getPosX() const { return posX; }
        int getPosY() const { return posY; }
        void setPlayerId(int id) { playerId = id; }
        int getPlayerId() const { return playerId; }
        sf::Sprite &getSprite() { return m_sprite; }

    private:
        sf::Sprite m_sprite; // Sprite for the player
        sf::Texture m_texture; // Texture for the player sprite
        int posX;
        int posY;
        int playerId;
};

#endif /* !PLAYER_HPP_ */
