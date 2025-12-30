/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player
*/

#include "Player.hpp"

Player::Player()
{
    m_texture.loadFromFile("2Dassets/player.png");
    m_sprite.setTexture(m_texture);
}

Player::~Player()
{
}

void Player::draw(sf::RenderWindow& window, float x, float y)
{
    m_sprite.setPosition(x, y);
    window.draw(m_sprite);
}

void Player::setScale(float x, float y)
{
    m_sprite.setScale(x, y);
}
