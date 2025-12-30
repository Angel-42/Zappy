/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RectangelButton.hpp
*/

#pragma once
#include "IButton.hpp"
#include <SFML/Graphics.hpp>
#include <string>

#pragma once

#include "IButton.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class RectangleButton : public IButton, public sf::Drawable
{
public:
    RectangleButton(const sf::Vector2f &size, const std::string &text = "");

    void setPosition(const sf::Vector2f &pos) override;
    void setSize(const sf::Vector2f &size) override;
    void setText(const std::string &text) override;
    void setCallback(std::function<void()> callback) override;

    void handleEvent(const sf::Event &event) override;
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    std::function<void()> m_callback;
    bool m_hovered = false;
};
