/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Slider.hpp
*/

#pragma once

#include "ISlider.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <cmath>

class Slider : public ISlider
{
public:
    Slider(float minValue, float maxValue, float initialValue = 0.0f);

    void setPosition(const sf::Vector2f &pos) override;
    void setSize(const sf::Vector2f &size) override;
    void setValue(float value) override;
    float getValue() const override;

    void handleEvent(const sf::Event &event) override;
    void draw(sf::RenderTarget &target) const override;

    void setCallback(std::function<void(float)> callback);

private:
    float m_min, m_max, m_value;
    sf::Vector2f m_position, m_size;
    sf::RectangleShape m_bar;
    sf::CircleShape m_knob;
    bool m_dragging = false;
    std::function<void(float)> m_callback;

    float valueToX(float value) const;
    float xToValue(float x) const;
};
