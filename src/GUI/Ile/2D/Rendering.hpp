/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Rendering
*/

#ifndef RENDERING_HPP_
#define RENDERING_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Food.hpp"
#include "Player.hpp"

#define SCALE_X this->m_window.getSize().x / 800.0f
#define SCALE_Y this->m_window.getSize().y / 600.0f

enum Drawables {
    PLAYER,
    FOOD,
    EMPTY,
};

class Rendering {
    public:
        Rendering();
        ~Rendering();
        void display();
        void handleEvents();
        void update();
        bool isOpen() const { return m_window.isOpen(); }
        void spritesetscale(float x, float y);
        void setMapSize(int size);
        void setPlayerCount(int count);
        void setFoodCount(int count);
        void spawnFood();
        void spawnPlayer();
        void displayMap();
        void initiateDrawables();

    private:
        sf::RenderWindow m_window; // Main window for rendering
        sf::View m_view;           // View for camera control
        sf::Clock m_clock;         // Clock for frame timing
        sf::Event m_event;         // Event handling
        //sf::Font m_font;           // Font for text rendering
        sf::Texture waterTexture;     // Texture for sprites
        sf::Sprite waterSprite;       // Sprite for rendering images
        sf::IntRect waterRect;       // Rectangle for sprite texture
        sf::Texture IslandTexture; // Texture for island background
        sf::Sprite IslandSprite;   // Sprite for island background
        sf::Music bgMusic;         // Music for background
        std::vector<int> m_map; // Vector to hold map data
        int size = 0; // Size of the map
        std::vector<Player> players;
        std::vector<Food> items;
        int playerCount = 0;
        int foodCount = 0;
        bool displayTile = false;
};

#endif /* !RENDERING_HPP_ */
