/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Renderer3D.cpp
*/

#include "Renderer3D.hpp"
#include <glm/gtx/string_cast.hpp>
#include <execinfo.h> // For backtrace

// SphereMesh implementation
SphereMesh::SphereMesh(float r, int sect, int st) 
    : radius(r), sectors(sect), stacks(st), 
      VAO(0), VBO(0), EBO(0), indexCount(0) {
}

SphereMesh::~SphereMesh() {
    cleanup();
}

void SphereMesh::generate(int resolution) {
    vertices.clear();
    indices.clear();            
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * M_PI / sectors;
            
            // Position
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            
            float nx = x / radius;
            float ny = y / radius;
            float nz = z / radius;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
            
            float s = (float)j / sectors;
            float t = (float)i / stacks;
            vertices.push_back(s);
            vertices.push_back(t);
        }
    }
    
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;
        
        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    
    indexCount = indices.size();
}

void SphereMesh::uploadToGPU() {
    if (initialized) {
        cleanup();
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    initialized = true;
}

void SphereMesh::cleanup() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    initialized = false;
}

void SphereMesh::draw() {
    if (initialized) {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

// Renderer3D implementation
Renderer3D::Renderer3D(sf::RenderWindow &window)
    : m_window(window)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    std::string vertSrc = ShaderFileLoader::load("Shaders/shader.vert");
    std::string fragSrc = ShaderFileLoader::load("Shaders/shader.frag");

    (void)m_shaderManager.load("normal", "Shaders/shader.vert", "Shaders/shader.frag");
    (void)m_shaderManager.load("water", "Shaders/o_water.vert", "Shaders/o_water.frag");
    (void)m_shaderManager.load("empty", "Shaders/empty.vert", "Shaders/empty.frag");
    
    // Create a solid color shader for players - use files instead of inline source
    std::string solidVert = 
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\n";
        
    std::string solidFrag = 
        "#version 330 core\n"
        "uniform vec3 solidColor;\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(solidColor, 1.0);\n"
        "}\n";
    
    // Create the shader directly instead of using loadFromSource
    m_solidColorShader = std::make_unique<GLShader>(solidVert.c_str(), solidFrag.c_str());

    shader = std::make_unique<GLShader>(vertSrc.c_str(), fragSrc.c_str());
    camera = std::make_unique<Camera>(
        glm::vec3(0.0f, 1.8f, 2.0f),
        glm::vec3(0.0f, 0.1f, 0.0f),
        -90.0f,
        0.0f);
}

void Renderer3D::initialize()
{
    int gridSize = 30; // put as args
    m_terrainMesh.generate(gridSize);
    m_terrainMesh.uploadToGPU();

    m_waterMesh.generate(100);
    m_waterMesh.uploadToGPU();

    float gridTileSize = 2.0f / gridSize;
    glm::vec3 gridPosition(-1.0f, 0.01f, -1.0f);
    if (m_grid)
        delete m_grid;
    m_grid = new Grid(gridSize, gridSize, gridTileSize, gridPosition);

    // --- Texture Loading ---
    m_sandTexture = m_textureManager.load("asset/Island/sand.jpg");
    if (m_sandTexture == 0)
        m_sandTexture = m_proceduralTextureGenerator.createDummyTexture(240, 210, 160);

    m_grassTexture = m_textureManager.load("asset/Island/grass.jpg");
    if (m_grassTexture == 0)
        m_grassTexture = m_proceduralTextureGenerator.createDummyTexture(60, 170, 60);

    m_rockTexture = m_textureManager.load("asset/Island/rock.jpg");
    if (m_rockTexture == 0)
        m_rockTexture = m_proceduralTextureGenerator.createDummyTexture(120, 120, 120);

    m_waterNormalMap = m_textureManager.load("asset/water_asset/normal2.jpg");
    if (m_waterNormalMap == 0)
        m_waterNormalMap = m_proceduralTextureGenerator.createDummyTexture(128, 128, 255);

    m_waterCartoonTexture = m_textureManager.load("asset/water_asset/water_cartoon.png");
    if (m_waterCartoonTexture == 0)
        m_waterCartoonTexture = m_proceduralTextureGenerator.createProceduralCartoonWater();

    m_waterDudvMap = m_textureManager.load("asset/water_asset/dudv.jpg");
    if (m_waterDudvMap == 0)
        m_waterDudvMap = m_proceduralTextureGenerator.createProceduralDudvMap();

    // --- Shader Setup ---
    GLShader *water_shader = m_shaderManager.get("water");
    if (!water_shader)
        return;
    water_shader->use();

    glUniform1i(glGetUniformLocation(water_shader->id(), "sandTexture"), 0);
    glUniform1i(glGetUniformLocation(water_shader->id(), "grassTexture"), 1);
    glUniform1i(glGetUniformLocation(water_shader->id(), "waterTexture"), 2);
    glUniform1i(glGetUniformLocation(water_shader->id(), "dudvMap"), 3);

    // Prevent players from being automatically cleared
    setPreventPlayerClearing(true);
}

void Renderer3D::render()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLShader *waterShader = m_shaderManager.get("water");
    if (!waterShader)
        return;
    waterShader->use();

    // Camera matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(m_window.getSize().x) / static_cast<float>(m_window.getSize().y),
        0.1f, 100.f);

    glUniformMatrix4fv(glGetUniformLocation(waterShader->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(waterShader->id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(waterShader->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(glGetUniformLocation(waterShader->id(), "time"), m_clock.getElapsedTime().asSeconds());

    // Bind textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_sandTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_grassTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_waterCartoonTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_waterDudvMap);

    glDisable(GL_CULL_FACE);

    // Draw terrain
    glUniform1i(glGetUniformLocation(shader->id(), "isCircle"), GL_FALSE);
    m_terrainMesh.draw();

    glEnable(GL_CULL_FACE);

    // Draw water
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_waterMesh.draw();
    glDisable(GL_BLEND);

    // Draw grid if enabled
    if (m_grid)
    {
        GLShader *emptyShader = m_shaderManager.get("empty");
        if (emptyShader)
        {
            emptyShader->use();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_grid->position);
            glm::mat4 view = camera->getViewMatrix();
            glm::mat4 projection = glm::perspective(
                glm::radians(45.0f),
                static_cast<float>(m_window.getSize().x) / static_cast<float>(m_window.getSize().y),
                0.1f, 100.f);

            glUniformMatrix4fv(glGetUniformLocation(emptyShader->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(emptyShader->id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(emptyShader->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        }
        glDisable(GL_CULL_FACE);
        m_grid->draw();
        glEnable(GL_CULL_FACE);
    }

    // Draw players - Use our solid color shader directly
    if (m_solidColorShader) {
        m_solidColorShader->use();
        
        // Reuse existing view and projection matrices
        glUniformMatrix4fv(glGetUniformLocation(m_solidColorShader->id(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(m_solidColorShader->id(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        // Draw regular players
        for (const auto& player : m_players) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, player->getPosition());
            model = glm::scale(model, player->getScale());
            
            glUniformMatrix4fv(glGetUniformLocation(m_solidColorShader->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            // Set player color based on team
            std::string team = player->getTeam();
            glm::vec3 color;
            
            if (team == "Team1")
                color = glm::vec3(1.0f, 0.0f, 0.0f); // Pure red for Team1
            else if (team == "Team2") 
                color = glm::vec3(0.0f, 0.0f, 1.0f); // Pure blue for Team2
            else if (team == "Team3")
                color = glm::vec3(0.0f, 1.0f, 0.0f); // Pure green for Team3
            else
                color = glm::vec3(1.0f, 0.0f, 0.0f); // Default to red
                
            glUniform3fv(glGetUniformLocation(m_solidColorShader->id(), "solidColor"), 1, glm::value_ptr(color));
            
            player->draw();
        }
        
        // Draw static players with the same shader
        glDisable(GL_DEPTH_TEST);
        
        for (const auto& player : m_staticPlayers) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, player->getPosition());
            model = glm::scale(model, player->getScale());
            
            glUniformMatrix4fv(glGetUniformLocation(m_solidColorShader->id(), "model"), 1, GL_FALSE, glm::value_ptr(model));
            
            glm::vec3 color = player->getAuraColor();
            glUniform3fv(glGetUniformLocation(m_solidColorShader->id(), "solidColor"), 1, glm::value_ptr(color));
            
            player->draw();
        }
        
        glEnable(GL_DEPTH_TEST);
    }
    
    glBindVertexArray(0);
}

void Renderer3D::update(float deltaTime)
{
    float cameraSpeed = 1.0f;
    float dx = 0.0f, dy = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        dy += 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        dy -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        dx -= 1.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        dx += 1.0f;

    sf::Vector2i windowCenter(m_window.getSize().x / 2, m_window.getSize().y / 2);
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);

    float xoffset = 0.0f;
    float yoffset = 0.0f;
    if (!firstMouse)
    {
        xoffset = mousePos.x - windowCenter.x;
        yoffset = windowCenter.y - mousePos.y;
    }
    else
    {
        firstMouse = false;
    }

    camera->update(dx, dy, cameraSpeed * deltaTime, xoffset, yoffset);
    sf::Mouse::setPosition(windowCenter, m_window);

    glm::mat4 view = camera->getViewMatrix();
    glm::vec3 lookAt = glm::vec3(view[3]) + glm::vec3(view[0][2], view[1][2], view[2][2]);
    
    static bool testModeEnabled = false; // Disable test mode by default
    static float testTimer = 0.0f;
    static int testPlayerId = 1;
    
    // Handle toggle with P key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && testTimer > 0.5f) {
        testModeEnabled = !testModeEnabled;
        testTimer = 0.0f;
        if (!testModeEnabled) {
            clearPlayers();
        }
    }
    
    testTimer += deltaTime;
}

void Renderer3D::shutdown()
{
    m_terrainMesh.cleanup();
    m_waterMesh.cleanup();
    if (shader)
        shader.reset();
    if (camera)
        camera.reset();
    if (m_grid)
    {
        delete m_grid;
        m_grid = nullptr;
    }
    glDeleteTextures(1, &m_sandTexture);
    glDeleteTextures(1, &m_grassTexture);
    glDeleteTextures(1, &m_rockTexture);
    glDeleteTextures(1, &m_waterNormalMap);
    glDeleteTextures(1, &m_waterCartoonTexture);
    glDeleteTextures(1, &m_waterDudvMap);
    m_textureManager.clear();
}

glm::vec3 Renderer3D::gridToWorld(int i, int j, float y) const
{
    if (!m_grid)
        return glm::vec3(0.0f);

    int clampedI = std::max(0, std::min(i, m_grid->getHeight() - 1));
    int clampedJ = std::max(0, std::min(j, m_grid->getWidth() - 1));

    float x = m_grid->position.x + (clampedJ + 0.5f) * m_grid->getTileSize();
    float z = m_grid->position.z + (clampedI + 0.5f) * m_grid->getTileSize();
    
    // If y is very close to 0, use the grid surface height
    if (std::abs(y) < 0.001f) {
        y = m_grid->position.y + 0.01f; // Just slightly above grid to avoid z-fighting
    }
    
    return glm::vec3(x, y, z);
}

void Renderer3D::addPlayer(int id, const glm::vec3 &position, const std::string &team, int level)
{
    // First check if we already have a player with this ID to avoid duplicates
    auto it = std::find_if(m_players.begin(), m_players.end(), 
                        [id](const auto& player) { 
                            return player->getId() == id; 
                        });
    
    if (it != m_players.end()) {
        updatePlayer(id, position, level);
        return;
    }

    // Ensure Y position is at grid level
    glm::vec3 adjustedPosition = position;
    adjustedPosition.y = m_grid->position.y + 0.01f; // Just above grid surface
    
    // Make players significantly larger
    float gridTileSize = m_grid ? m_grid->getTileSize() : 0.1f;
    float baseScale = gridTileSize * 0.7f;
    float scaleFactor = baseScale + (level - 1) * 0.05f;
    
    auto player = std::make_unique<renderer3d::Player>(
        adjustedPosition,
        glm::vec3(0.0f),
        glm::vec3(scaleFactor),
        team);

    // Set the player ID
    player->setId(id);

    // Colors for the three teams only
    if (team == "Team1")
        player->setAuraColor(glm::vec3(1.0f, 0.0f, 0.0f)); // Pure red for Team1
    else if (team == "Team2") 
        player->setAuraColor(glm::vec3(0.0f, 0.0f, 1.0f)); // Pure blue for Team2
    else if (team == "Team3")
        player->setAuraColor(glm::vec3(0.0f, 1.0f, 0.0f)); // Pure green for Team3
    else
        // Default to red for any unknown team
        player->setAuraColor(glm::vec3(1.0f, 0.0f, 0.0f));

    float radius = scaleFactor;
    IMesh* sphereMesh = createSphereMesh(radius, 20, 20);
    player->setMesh(sphereMesh);

    m_players.push_back(std::move(player));
}

void Renderer3D::updatePlayer(int id, const glm::vec3 &position, int level)
{
    // Find the player with the given ID
    auto it = std::find_if(m_players.begin(), m_players.end(), 
                          [id](const auto& player) { 
                              return player->getId() == id;
                          });
    
    if (it != m_players.end()) {
        // Update existing player - ensure Y position is at grid level
        glm::vec3 adjustedPosition = position;
        adjustedPosition.y = m_grid->position.y + 0.01f; // Just above grid surface
        
        (*it)->setPosition(adjustedPosition);
        
        // Update scale if level changed - use the same scale calculation as in addPlayer
        float gridTileSize = m_grid ? m_grid->getTileSize() : 0.1f;
        float baseScale = gridTileSize * 0.7f;
        float scaleFactor = baseScale + (level - 1) * 0.05f;
        (*it)->setScale(glm::vec3(scaleFactor));
    } else {
        // Player not found - create a new one
        // Default to Team1 if not specified
        addPlayer(id, position, "Team1", level);
    }
}

void Renderer3D::clearPlayers()
{
    // Only clear players if prevention is not enabled
    if (!m_preventClearing) {
        m_players.clear();
    }
}

IMesh* Renderer3D::createSphereMesh(float radius, int sectors, int stacks)
{
    SphereMesh* mesh = new SphereMesh(radius, sectors, stacks);
    mesh->generate(sectors); 
    mesh->uploadToGPU();
    return mesh;
}
