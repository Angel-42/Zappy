/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** CircleButton.hpp
*/

#pragma once
#include "IButton.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>

class CircleButton : public IButton, public sf::Drawable
{
public:
    CircleButton(float radius, const std::string &text = "");

    void setPosition(const sf::Vector2f &pos) override;
    void setSize(const sf::Vector2f &size) override;
    void setText(const std::string &text) override;
    void setCallback(std::function<void()> callback) override;

    void handleEvent(const sf::Event &event) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::CircleShape m_shape;
    sf::Text m_text;
    std::function<void()> m_callback;
    bool m_hovered = false;
};
