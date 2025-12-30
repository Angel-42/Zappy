/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** main.cpp
*/

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <fstream>
#include <sstream>
#include <cstring>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Game.hpp"
#include "GameListener.hpp"
#include "Renderer/3D/Camera/Camera.hpp"
#include "Renderer/Utils/GLShader.hpp"
#include "Renderer/Utils/ShaderFileLoader.hpp"

#include "Renderer/State/StateMachine.hpp"
#include "Renderer/State/MenuState.hpp"
#include "Renderer/State/GameState.hpp"
#include "Renderer/State/2DState.hpp"

#include "../ClientAPI/ClientAPIWrapper.hpp"
#include "../ClientAPI/Model/GameState.hpp"

int main(int argc, char **argv)
{
    std::string port;
    std::string machine;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
        {
            port = argv[++i];
        }
        else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc)
        {
            machine = argv[++i];
        }
    }

    if (port.empty() || machine.empty())
    {
        std::cerr << "USAGE: " << argv[0] << " -p port -h machine" << std::endl;
        return 84;
    }

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = 0;

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Zappy OpenGL 3D", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    auto &api = clientapi::ClientAPIWrapper::getInstance();
    if (!api.connect(machine, std::stoi(port))) // Use machine and port from command line
    {
        std::cerr << "Failed to connect to Zappy server at " << machine << ":" << port << std::endl;
    }
    else
    {
        api.sendGraphicCommand();
        std::cout << "Connected to Zappy server as graphical client" << std::endl;
    }

    Game game;
    auto listener = std::make_shared<GameListener>(window);
    game.getDispatcher()->addListener(listener);

    StateMachine stateMachine;
    stateMachine.pushState(std::make_unique<GameState>(window, api));

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            game.handleSfmlEvent(event);
            stateMachine.handleEvent(event);

            // Add this block to handle O and P key presses
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::O)
                {
                    stateMachine.clearStates();
                    stateMachine.pushState(std::make_unique<TwoDState>(window, api));
                }
                if (event.key.code == sf::Keyboard::P)
                {
                    stateMachine.clearStates();
                    stateMachine.pushState(std::make_unique<GameState>(window, api));
                }
            }
        }

        stateMachine.update(deltaTime);
        stateMachine.render();

        window.display();
    }

    return 0;
}