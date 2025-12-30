/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** MenuState.cpp
*/

#include "MenuState.hpp"
#include "StateMachine.hpp"
#include "../../SFMLUtils/Button/RectangleButton.hpp"
#include <GL/glew.h>

MenuState::MenuState(sf::RenderWindow &window) : window(window)
{
    uiLayer.clear();
    std::cout << "> Initializing Menu State" << std::endl;

    if (!font.loadFromFile("../../arial.ttf"))
    {
        std::cerr << "Failed to load font (MenuState)" << std::endl;
        throw std::runtime_error("Failed to load font");
    }

    logoTexture = std::make_shared<sf::Texture>();
    if (!logoTexture->loadFromFile("../../logo.png"))
    {
        std::cerr << "Failed to load logo image (MenuState)" << std::endl;
        throw std::runtime_error("Failed to load logo image");
    }
    auto logoSprite = std::make_shared<sf::Sprite>(*logoTexture);
    logoSprite->setScale(0.3f, 0.3f); // Scale down the logo if needed
    logoSprite->setPosition(sf::Vector2f(window.getSize().x / 2.f - logoSprite->getGlobalBounds().width / 2.f, 0.f));
    uiLayer.addElement(logoSprite);

    // connect box
    auto connectBox = std::make_shared<sf::RectangleShape>(sf::Vector2f(300.f, 150.f));
    connectBox->setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent dark box
    connectBox->setPosition(sf::Vector2f(window.getSize().x / 2.f - connectBox->getSize().x / 2.f, window.getSize().y / 2.f - connectBox->getSize().y / 2.f));
    uiLayer.addElement(connectBox);

    // team box
    auto teamBox = std::make_shared<sf::RectangleShape>(sf::Vector2f(300.f, 100.f));
    teamBox->setFillColor(sf::Color(50, 50, 50, 200)); // Semi-transparent dark box
    teamBox->setPosition(sf::Vector2f(window.getSize().x / 2.f - teamBox->getSize().x / 2.f, window.getSize().y / 2.f + connectBox->getSize().y / 2.f + 20.f));
    uiLayer.addElement(teamBox);

    // host text input (just text for now)
    auto hostText = std::make_shared<sf::Text>("Host :", font, 20);
    hostText->setFillColor(sf::Color::White);
    hostText->setPosition(sf::Vector2f(connectBox->getPosition().x + 10.f, connectBox->getPosition().y + 10.f));
    uiLayer.addElement(hostText);

    // port text input (just text for now)
    auto portText = std::make_shared<sf::Text>("Port :", font, 20);
    portText->setFillColor(sf::Color::White);
    portText->setPosition(sf::Vector2f(connectBox->getPosition().x + 10.f, connectBox->getPosition().y + 40.f));
    uiLayer.addElement(portText);

    // team name text input (just text for now)
    auto teamText = std::make_shared<sf::Text>("Team Name :", font, 20);
    teamText->setFillColor(sf::Color::White);
    teamText->setPosition(sf::Vector2f(teamBox->getPosition().x + 10.f, teamBox->getPosition().y + 10.f));
    uiLayer.addElement(teamText);

    // Exit button
    auto button = std::make_shared<RectangleButton>(sf::Vector2f(150.f, 50.f), "Exit");
    button->setPosition(sf::Vector2f(
        teamBox->getPosition().x + (teamBox->getSize().x) / 2.f,
        teamBox->getPosition().y + teamBox->getSize().y + 20.f));
    button->setCallback([&window]()
                        { window.close(); });
    uiLayer.addElement(button);

    // Settings button
    auto settingsButton = std::make_shared<RectangleButton>(sf::Vector2f(150.f, 50.f), "Settings");
    settingsButton->setPosition(sf::Vector2f(
        teamBox->getPosition().x + (teamBox->getSize().x - 300.f) / 2.f,
        teamBox->getPosition().y + teamBox->getSize().y + 20.f));
    settingsButton->setCallback([]()
                                { std::cout << "Settings button clicked" << std::endl; });
    uiLayer.addElement(settingsButton);
}

void MenuState::onEnter(StateMachine &)
{
    std::cout << "> Entering Menu State" << std::endl;
}

void MenuState::onExit(StateMachine &)
{
    std::cout << "> Exiting Menu State" << std::endl;
    uiLayer.clear();
    logoTexture.reset();
    font = sf::Font();
}

void MenuState::update(StateMachine &, float)
{
}

void MenuState::render(StateMachine &)
{
    window.resetGLStates();
    window.pushGLStates();
    window.clear(sf::Color::Black);
    uiLayer.draw(window);
    window.popGLStates();
}

void MenuState::handleEvent(StateMachine &, const sf::Event &event)
{
    uiLayer.handleEvent(event);
}
