/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Rendering
*/

#include "Rendering.hpp"
#include <iostream>

Rendering::Rendering()
{
    // Initialize the main window
    m_window.create(sf::VideoMode(800, 600), "Zappy Rendering");
    m_window.setFramerateLimit(60);

    // Load textures and sprites
    if (!waterTexture.loadFromFile("waterCartoon.png")) {
        // Handle error
    }
    waterSprite.setTexture(waterTexture);
    waterRect = sf::IntRect(0, 0, 800, 600);
    waterSprite.setTextureRect(waterRect);

    if (!IslandTexture.loadFromFile("map.png")) {
        // Handle error
    }
    IslandSprite.setTexture(IslandTexture);

    // Load background music
    if (!bgMusic.openFromFile("music.ogg")) {
        // Handle error
    }
    bgMusic.setLoop(true);
    bgMusic.setVolume(20); // Set volume to 20%
    // bgMusic.play();

    // Set the initial view
    m_view.setSize(800, 600);
    m_view.setCenter(400, 300);
    m_window.setView(m_view);
}

Rendering::~Rendering()
{
}

void Rendering::display()
{
    m_window.clear();
    m_window.draw(waterSprite);
    m_window.draw(IslandSprite);
    this->displayMap();
    m_window.display();
}

void Rendering::handleEvents()
{
    while (m_window.pollEvent(m_event)) {
        if (m_event.type == sf::Event::Closed) {
            m_window.close();
        }
        if (m_event.type == sf::Event::KeyPressed) {
            if (m_event.key.code == sf::Keyboard::Escape) {
                m_window.close();
            }
            if (m_event.key.code == sf::Keyboard::T) {
                displayTile = !displayTile;
            }
        }
        if (m_event.type == sf::Event::MouseButtonPressed) {
            if (m_event.mouseButton.button == sf::Mouse::Left) {
                std::cout << "Mouse position: "
                          << m_event.mouseButton.x << ", "
                          << m_event.mouseButton.y << std::endl;
            }
        }
    }
}

void Rendering::spritesetscale(float x, float y)
{
    waterSprite.setScale(x / 800, y / 600);
    IslandSprite.setScale(x / 800, y / 600);
    m_view.setSize(x, y);
    m_view.setCenter(x / 2, y / 2);
    m_window.setView(m_view);
    for (auto &player : players) {
        player.setScale(0.23f / size * SCALE_X, 0.17f / size * SCALE_Y);
    }
    for (auto &item : items) {
        item.setScale(0.226f / size * SCALE_X, 0.3f / size * SCALE_Y);
    }
}

void Rendering::update()
{
    this->handleEvents();
    this->spritesetscale(this->m_window.getSize().x, this->m_window.getSize().y);
    this->display();
    this->waterRect.left += 1;
    if (waterRect.left == 1000) {
        waterRect.left = 0;
    }
    waterSprite.setTextureRect(waterRect);
}

void Rendering::setMapSize(int size)
{
    this->size = size;
    m_map.resize(size * size);
    for (int i = 0; i < size * size; ++i) {
        m_map[i] = EMPTY;
    }
}

void Rendering::setPlayerCount(int count)
{
    playerCount = count;
    players.resize(count);
}

void Rendering::setFoodCount(int count)
{
    foodCount = count;
    items.resize(count);
}

void Rendering::spawnFood()
{
    items.push_back(Food());
}

void Rendering::spawnPlayer()
{
    players.push_back(Player());
}

void Rendering::displayMap()
{
    int indexPlayer = 0;
    int indexFood = 0;

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int index = i * size + j;
            float cubesize = (568 - 240) / this->size;
            int posx = (240 * SCALE_X) + j * cubesize * SCALE_X;
            int posy = (112 * SCALE_Y) + i * cubesize * SCALE_Y;

            if (displayTile) {
                sf::RectangleShape tile(sf::Vector2f(cubesize * SCALE_X, cubesize * SCALE_Y));
                tile.setPosition(posx, posy);
                if (index % 2 == 0) {
                    tile.setFillColor(sf::Color(200, 200, 200, 125));
                } else {
                    tile.setFillColor(sf::Color(150, 150, 150, 125));
                }
                m_window.draw(tile);
            }
            if (m_map[index] == PLAYER) {
                players[indexPlayer].draw(m_window, posx, posy);
                indexPlayer++;
            } else if (m_map[index] == FOOD) {
                items[indexFood].draw(m_window, posx, posy);
                indexFood++;
            }
        }
    }
}

void Rendering::initiateDrawables()
{
    for (int i = this->players.size(); i < playerCount; ++i) {
        players.emplace_back();
    }
    for (int i = this->items.size(); i < foodCount; ++i) {
        items.emplace_back();
    }
    for (int i = 0; i < size * size; ++i) {
        m_map[i] = EMPTY;
    }
    for (int i = 0; i < playerCount; ++i) {
        int x = rand() % size;
        int y = rand() % size;
        while (m_map[y * size + x] != EMPTY) {
            x = rand() % size;
            y = rand() % size;
        }
        m_map[y * size + x] = PLAYER;
    }
    for (int i = 0; i < foodCount; ++i) {
        int x = rand() % size;
        int y = rand() % size;
        while (m_map[y * size + x] != EMPTY) {
            x = rand() % size;
            y = rand() % size;
        }
        m_map[y * size + x] = FOOD;
    }
}
