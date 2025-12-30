/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Food
*/

#include "Food.hpp"

Food::Food()
{
    if (!m_texture.loadFromFile("food.png")) {
        // Handle error
    }
    m_sprite.setTexture(m_texture);
}

Food::~Food()
{
}

void Food::draw(sf::RenderWindow& window, float x, float y)
{
    m_sprite.setPosition(x, y);
    window.draw(m_sprite);
}

void Food::setScale(float x, float y)
{
    m_sprite.setScale(x, y);
}
