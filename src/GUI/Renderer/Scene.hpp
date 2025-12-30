/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Scene.hpp
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "3D/Renderer3D.hpp"

/**
 * @class Scene
 * @brief Manages the rendering mode (2D or 3D) and delegates rendering tasks.
 *
 * The Scene class is responsible for initializing, updating, rendering, and cleaning up
 * either a 2D or 3D renderer, depending on the selected mode.
 */
class Scene
{
public:
    /**
     * @enum RenderMode
     * @brief Specifies the rendering mode for the scene.
     */
    enum class RenderMode
    {
        Mode2D, ///< Use 2D rendering.
        Mode3D  ///< Use 3D rendering.
    };

    /**
     * @brief Constructs a Scene with the specified rendering mode.
     * @param mode The rendering mode (2D or 3D).
     * @param window Reference to the SFML render window.
     *
     * Initializes the appropriate renderer (2D or 3D) based on the mode.
     */
    Scene(RenderMode mode, sf::RenderWindow &window)
    {
        if (mode == RenderMode::Mode3D)
        {
            std::cout << "Initializing 3D Renderer" << std::endl;
            renderer = std::make_unique<Renderer3D>(window);
        }
    }

    /**
     * @brief Gets the active renderer.
     * @return Pointer to the IRenderer instance.
     */
    IRenderer *getRenderer() const { return renderer.get(); }

    /**
     * @brief Initializes the scene and its renderer.
     *
     * Should be called before rendering or updating the scene.
     */
    void initialize();

    /**
     * @brief Renders the scene using the selected renderer.
     * @param window Reference to the SFML render window.
     */
    void render(sf::RenderWindow &window);

    /**
     * @brief Updates the scene state.
     * @param deltaTime Time elapsed since the last update (in seconds).
     *
     * Typically called once per frame to update animations or logic.
     */
    void update(float deltaTime);

    /**
     * @brief Cleans up resources used by the scene and renderer.
     *
     * Should be called before destroying the scene.
     */
    void cleanup();

private:
    std::unique_ptr<IRenderer> renderer; ///< Pointer to the active renderer (2D or 3D).
};
