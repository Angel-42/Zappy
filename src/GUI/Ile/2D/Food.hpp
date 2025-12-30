/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Food
*/

#ifndef FOOD_HPP_
#define FOOD_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

class Food {
    public:
        Food();
        ~Food();
        void draw(sf::RenderWindow &window, float x, float y);
        void setScale(float x, float y);

    private:
        sf::Sprite m_sprite; // Sprite for the food item
        sf::Texture m_texture; // Texture for the food item sprite
};

#endif /* !FOOD_HPP_ */
