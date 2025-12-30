/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Tile2D.hpp
*/

#pragma once

#include "ITile.hpp"
#include <SFML/Graphics.hpp>

class Tile2D : public ITile
{
public:
    Tile2D(const glm::vec3 &pos, const sf::Vector2f &size, const sf::Color &color = sf::Color::White);

    void setPosition(const glm::vec3 &pos) override;
    const glm::vec3 &getPosition() const override;
    void draw() override;

    void setRenderTarget(sf::RenderTarget *target);

private:
    glm::vec3 _position;
    sf::RectangleShape _shape;
    sf::RenderTarget *_target = nullptr;
};
