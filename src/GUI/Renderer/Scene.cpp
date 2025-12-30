/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Scene.cpp
*/

#include "Scene.hpp"
#include <SFML/OpenGL.hpp>

void Scene::initialize()
{
    std::cout << "> Initializing Scene" << std::endl;
    if (this->renderer)
    {
        this->renderer->initialize();
    }
    else
    {
        std::cerr << "Renderer not initialized!" << std::endl;
    }
}

void Scene::render(sf::RenderWindow &window)
{
    if (this->renderer)
    {
        this->renderer->render();
    }
    else
    {
        std::cerr << "Renderer not initialized!" << std::endl;
    }
}

void Scene::update(float deltaTime)
{
    if (this->renderer)
    {
        this->renderer->update(deltaTime);
    }
    else
    {
        std::cerr << "Renderer not initialized!" << std::endl;
    }
}

void Scene::cleanup()
{
    std::cout << "> Cleaning up Scene" << std::endl;
    if (this->renderer)
    {
        this->renderer->shutdown();
    }
    else
    {
        std::cerr << "Renderer not initialized!" << std::endl;
    }
}
