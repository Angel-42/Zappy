/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Text.hpp
*/

#pragma once

#include "IText.hpp"

class Text : public IText
{
public:
    Text();
    ~Text() override = default;

    void setPosition(const sf::Vector2f &pos) override;
    void setString(const std::string &str) override;
    void setFont(const sf::Font &font) override;
    void setCharacterSize(unsigned int size) override;
    void setFillColor(const sf::Color &color) override;

    void draw(sf::RenderTarget &target) const override;

private:
    sf::Text _text;
};
