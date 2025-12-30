/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Renderer3D.hpp
*/

#pragma once

#include <GL/glew.h>

#include "../IRenderer.hpp"
#include "../UILayer/UILayer.hpp"
#include "../Utils/GLShader.hpp"
#include "../3D/Camera/Camera.hpp"
#include "Grid/Grid.hpp"

#include "Objects/Terrain/TerrainMesh.hpp"
#include "Objects/Water/WaterMesh.hpp"
#include "Shaders/ShaderManager.hpp"
#include "Textures/TextureManager.hpp"
#include "Textures/ProceduralTextureGenerator.hpp"
#include "../Utils/ShaderFileLoader.hpp"
#include "Objects/Players/Player.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <memory>

/**
 * @class SphereMesh
 * @brief Implements a sphere mesh for rendering in 3D space.
 * 
 * This class creates a sphere with customizable radius, sectors, and stacks.
 * It handles all vertex/index generation and GPU resource management.
 * 
 * @inherits IMesh
 */
class SphereMesh : public IMesh {
private:
    GLuint VAO, VBO, EBO;
    int indexCount;
    float radius;
    int sectors, stacks;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    bool initialized = false;

public:
    /**
     * @brief Constructs a sphere mesh with specified parameters
     * @param r Radius of the sphere
     * @param sect Number of horizontal divisions (sectors)
     * @param st Number of vertical divisions (stacks)
     */
    SphereMesh(float r, int sect, int st);
    
    /**
     * @brief Destructor that cleans up GPU resources
     */
    ~SphereMesh() override;
    
    /**
     * @brief Generates vertex and index data for the sphere
     * @param resolution Determines the mesh detail (unused, uses sectors/stacks instead)
     */
    void generate(int resolution) override;
    
    /**
     * @brief Uploads generated mesh data to the GPU
     */
    void uploadToGPU() override;
    
    /**
     * @brief Cleans up GPU resources
     */
    void cleanup() override;
    
    /**
     * @brief Renders the sphere mesh
     */
    void draw() override;
};

/**
 * @class Renderer3D
 * @brief 3D renderer implementation for the GUI, handling OpenGL rendering and resource management.
 *
 * This class is responsible for initializing, updating, rendering, and shutting down the 3D rendering context.
 * It manages OpenGL resources, shaders, camera, terrain, water, and player rendering.
 *
 * @inherits IRenderer
 */
class Renderer3D : public IRenderer
{
public:
    /**
     * @brief Constructs a Renderer3D object.
     * @param window Reference to the SFML render window used for rendering.
     */
    Renderer3D(sf::RenderWindow &window);

    /**
     * @brief Initializes the renderer, loading resources and setting up OpenGL state.
     */
    void initialize() override;

    /**
     * @brief Renders the 3D scene, including terrain, water, and players.
     */
    void render() override;

    /**
     * @brief Updates the renderer state, including camera and animations.
     * @param deltaTime Time elapsed since the last update, in seconds.
     */
    void update(float deltaTime) override;

    /**
     * @brief Releases resources and shuts down the renderer.
     */
    void shutdown() override;

    /**
     * @brief Returns a reference to the UI layer for overlay rendering.
     * @return Reference to the UILayer object.
     */
    [[nodiscard]] UILayer &getUILayer();

    /**
     * Convert grid coordinates (i, j) to world position (center of tile).
     * @param i Row index (0 ... grid height - 1)
     * @param j Column index (0 ... grid width - 1)
     * @param y Optional height (default 0)
     * @return glm::vec3 world position
     */
    glm::vec3 gridToWorld(int i, int j, float y = 0.0f) const;

    /**
     * Adds a player to the renderer.
     * @param id Unique identifier for the player.
     * @param position World position of the player.
     * @param team Team name of the player.
     * @param level Player's level.
     */
    void addPlayer(int id, const glm::vec3 &position, const std::string &team, int level);

    /**
     * Updates an existing player's position and level.
     * @param id Unique identifier for the player.
     * @param position New world position of the player.
     * @param level New level of the player.
     */
    void updatePlayer(int id, const glm::vec3 &position, int level);

    /**
     * Clears all players from the renderer.
     */
    void clearPlayers();

    /**
     * Creates a sphere mesh for rendering.
     * @param radius Radius of the sphere
     * @param sectors Number of horizontal divisions
     * @param stacks Number of vertical divisions
     * @return Pointer to the created IMesh object
     */
    IMesh *createSphereMesh(float radius, int sectors, int stacks);

    /**
     * Adds a static player that won't be cleared by normal operations
     */
    void addStaticPlayer(int id, const glm::vec3 &position, const std::string &team, int level);

    /**
     * @brief Prevents or allows player clearing on the next frame
     * @param prevent If true, player clearing is prevented
     */
    void setPreventPlayerClearing(bool prevent) { m_preventClearing = prevent; }

private:
    // No documentation for private members by default.
    UILayer m_uiLayer;
    sf::RenderWindow &m_window;

    std::unique_ptr<GLShader> shader;
    std::unique_ptr<GLShader> m_solidColorShader; // Add this new shader for solid colors
    std::unique_ptr<Camera> camera;

    float lastFrameTime = 0.f;
    bool firstMouse = true;
    glm::vec2 lastMousePos = glm::vec2(0.0f);
    sf::Clock m_clock;

    // OpenGL handles
    GLuint m_sandTexture = 0, m_grassTexture = 0, m_rockTexture = 0;
    GLuint m_waterNormalMap = 0, m_waterCartoonTexture = 0, m_waterDudvMap = 0;

    TerrainMesh m_terrainMesh;
    WaterMesh m_waterMesh;
    Grid *m_grid = nullptr;

    ShaderManager m_shaderManager;
    TextureManager m_textureManager;
    ProceduralTextureGenerator m_proceduralTextureGenerator;

    std::vector<std::unique_ptr<renderer3d::Player>> m_players;
    std::vector<std::unique_ptr<renderer3d::Player>> m_staticPlayers; // These won't be cleared
    bool m_preventClearing = false; // Add this flag
};
