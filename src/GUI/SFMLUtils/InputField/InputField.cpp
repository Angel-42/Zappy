/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** InputField.cpp
*/

#include "InputField.hpp"
#include <algorithm>

using namespace gui;

InputField::InputField(const sf::Font &font,
                       const sf::Vector2f &position,
                       Style style)
    : style_{style}
{
    box_.setSize(style_.size);
    box_.setPosition(position);
    box_.setFillColor(style_.fillColor);
    box_.setOutlineColor(style_.borderColor);
    box_.setOutlineThickness(style_.borderThickness);

    text_.setFont(font);
    text_.setPosition(position + sf::Vector2f(8.f, 4.f));
    text_.setCharacterSize(style_.characterSize);
    text_.setFillColor(style_.textColor);
}

bool InputField::handleEvent(const sf::Event &ev)
{
    switch (ev.type)
    {
    case sf::Event::TextEntered:
    {
        const auto ch = static_cast<char>(ev.text.unicode);
        if (ch >= 32 && ch <= 126)
        {
            buffer_.push_back(ch);
            confirmed_ = false;
            updateGraphics_();
            return true;
        }
        break;
    }
    case sf::Event::KeyPressed:
    {
        if (ev.key.code == sf::Keyboard::BackSpace)
        {
            if (!buffer_.empty())
            {
                buffer_.pop_back();
                updateGraphics_();
            }
            return true;
        }
        if (ev.key.code == sf::Keyboard::Enter)
        {
            confirmed_ = true;
            if (onConfirm_)
                onConfirm_(buffer_);
            return true;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

bool InputField::confirmed() const noexcept { return confirmed_; }
const std::string &InputField::text() const noexcept { return buffer_; }

void InputField::reset(bool keepText) noexcept
{
    confirmed_ = false;
    if (!keepText)
        buffer_.clear();
    updateGraphics_();
}

void InputField::setOnConfirm(std::function<void(const std::string &)> cb)
{
    onConfirm_ = std::move(cb);
}

void InputField::updateGraphics_()
{
    text_.setString(buffer_);
}

void InputField::draw(sf::RenderTarget &tgt, sf::RenderStates st) const
{
    tgt.draw(box_, st);
    tgt.draw(text_, st);
}
