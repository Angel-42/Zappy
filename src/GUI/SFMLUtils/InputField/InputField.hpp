/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** InputField.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

namespace gui
{
    class InputField : public sf::Drawable
    {
    public:
        struct Style
        {
            sf::Vector2f size;
            float borderThickness;
            sf::Color borderColor;
            sf::Color fillColor;
            sf::Color textColor;
            unsigned characterSize;

            Style()
                : size(200.f, 32.f),
                  borderThickness(2.f),
                  borderColor(sf::Color::White),
                  fillColor(sf::Color(30, 30, 30)),
                  textColor(sf::Color::White),
                  characterSize(16)
            {
            }
        };

        InputField(const sf::Font &font,
                   const sf::Vector2f &position,
                   Style style = Style());
        bool handleEvent(const sf::Event &ev);
        [[nodiscard]] bool confirmed() const noexcept;
        [[nodiscard]] const std::string &text() const noexcept;
        void reset(bool keepText = false) noexcept;
        void setOnConfirm(std::function<void(const std::string &)> cb);

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
        void updateGraphics_();

        Style style_;
        sf::RectangleShape box_;
        sf::Text text_;
        std::string buffer_;
        bool confirmed_{false};
        std::function<void(const std::string &)> onConfirm_;
    };
} // namespace gui
