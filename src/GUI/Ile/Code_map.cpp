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

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

float smoothstep(float edge0, float edge1, float x)
{
    float t = std::max(0.0f, std::min(1.0f, (x - edge0) / (edge1 - edge0)));
    return t * t * (3.0f - 2.0f * t);
}

void generateCircle(std::vector<float> &vertices, std::vector<GLuint> &indices,
                    float radius, int segments, float height = 0.01f)
{
    int startIndex = vertices.size() / 6;

    vertices.push_back(0.0f);
    vertices.push_back(height);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);

    for (int i = 0; i < segments; i++)
    {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        vertices.push_back(x);
        vertices.push_back(height);
        vertices.push_back(z);
        vertices.push_back(0.5f);
        vertices.push_back(0.5f);
        vertices.push_back(0.0f);
    }

    for (int i = 0; i < segments; i++)
    {
        indices.push_back(startIndex);
        indices.push_back(startIndex + 1 + i);
        indices.push_back(startIndex + 1 + ((i + 1) % segments));
    }
}

GLuint loadTexture(const std::string &path);
GLuint createDummyTexture(int r, int g, int b);

float calculateTerrainHeight(float x, float z)
{
    float distanceFromCenter = sqrt(x * x + z * z);
    float height = 0.0f;
    float waterLevel = -0.05f;
    float minHeightAboveWater = 0.05f;
    if (distanceFromCenter < 0.8f)
    {
        float sphereRadius = 2.5f;
        float baseHeight = sphereRadius - sqrt(sphereRadius * sphereRadius - std::min(sphereRadius * sphereRadius, x * x + z * z));
        baseHeight = baseHeight * 0.3f + minHeightAboveWater + waterLevel;
        height = baseHeight;
        height += 0.12f * (0.8f - distanceFromCenter);
        height += 0.1f * sin(x * 6.0f) * cos(z * 6.0f);
        float smallFeatures = 0.05f * sin(x * 12.0f + 0.5f) * sin(z * 12.0f + 0.3f);
        height += std::max(smallFeatures, -0.03f);
        float centralMountain = 0.15f * exp(-8.0f * (x * x + z * z));
        height += centralMountain;
        float crater = std::max(static_cast<float>(-0.05f),
                                static_cast<float>(-0.06f * exp(-15.0f * ((x + 0.3f) * (x + 0.3f) + (z - 0.2f) * (z - 0.2f)))));
        height += crater;
        height = std::max(height, waterLevel + minHeightAboveWater);
    }
    return height;
}

// Helper: Create procedural DUDV map
GLuint createProceduralDudvMap(int width = 256, int height = 256)
{
    std::vector<unsigned char> data(width * height * 4);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char r = 128 + (unsigned char)(127.0f * sin(i * 0.1f) * cos(j * 0.1f));
            unsigned char g = 128 + (unsigned char)(127.0f * sin(j * 0.15f + 0.5f) * cos(i * 0.12f));
            int idx = (i * width + j) * 4;
            data[idx + 0] = r;
            data[idx + 1] = g;
            data[idx + 2] = 255;
            data[idx + 3] = 255;
        }
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return tex;
}

// Helper: Create procedural cartoon water texture
GLuint createProceduralCartoonWater(int width = 256, int height = 256)
{
    std::vector<unsigned char> data(width * height * 4);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float pattern = sin(j * 0.1f) * cos(i * 0.1f) * 0.5f + 0.5f;
            pattern *= sin(i * 0.05f + j * 0.05f) * 0.3f + 0.7f;
            unsigned char blue = static_cast<unsigned char>(180 + 75 * pattern);
            unsigned char green = static_cast<unsigned char>(130 + 60 * pattern);
            unsigned char red = static_cast<unsigned char>(30 + 40 * pattern);
            int idx = (i * width + j) * 4;
            data[idx + 0] = red;
            data[idx + 1] = green;
            data[idx + 2] = blue;
            data[idx + 3] = 255;
        }
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return tex;
}

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::RenderWindow window(sf::VideoMode(1920, 1200), "Zappy OpenGL 3D", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Check shader compilation and linking errors
    auto checkShaderErrors = [](GLuint shader, bool isProgram = false)
    {
        GLint success;
        char infoLog[512];

        if (!isProgram)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 512, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n"
                          << infoLog << std::endl;
            }
        }
        return success;
    };

    // Set back to the original gridSize for now
    const int gridSize = 16;
    std::vector<float> vertices;
    std::vector<GLuint> indices;

    float circleX = 0.0f;
    float circleZ = 0.0f;
    float moveSpeed = 0.01f;

    // Check if textures are loading correctly
    std::cout << "Loading textures..." << std::endl;

    // Load terrain textures with proper error handling
    GLuint sandTexture = loadTexture("../../../asset/Island/sand.jpg");
    if (sandTexture == 0)
    {
        std::cout << "Using dummy sand texture" << std::endl;
        sandTexture = createDummyTexture(240, 210, 160);
    }
    else
    {
        std::cout << "Sand texture loaded successfully" << std::endl;
    }

    GLuint grassTexture = loadTexture("../../../asset/Island/grass.jpg");
    if (grassTexture == 0)
    {
        std::cout << "Using dummy grass texture" << std::endl;
        grassTexture = createDummyTexture(60, 170, 60);
    }
    else
    {
        std::cout << "Grass texture loaded successfully" << std::endl;
    }

    GLuint rockTexture = loadTexture("../../../asset/Island/rock.jpg");
    if (rockTexture == 0)
    {
        std::cout << "Using dummy rock texture" << std::endl;
        rockTexture = createDummyTexture(120, 120, 120);
    }
    else
    {
        std::cout << "Rock texture loaded successfully" << std::endl;
    }

    // Water textures
    GLuint waterNormalMap = loadTexture("../../../asset/water_asset/normal2.jpg");
    if (waterNormalMap == 0)
    {
        std::cout << "Using dummy water normal map" << std::endl;
        waterNormalMap = createDummyTexture(128, 128, 255);
    }
    else
    {
        std::cout << "Water normal map loaded successfully" << std::endl;
    }

    // Try alternate paths for the cartoon texture
    GLuint waterCartoonTexture = loadTexture("../../../asset/water_asset/water_cartoon.png");
    if (waterCartoonTexture == 0)
    {
        waterCartoonTexture = loadTexture("../../../asset/water_asset/waterCartoon.png");
        if (waterCartoonTexture == 0)
        {
            std::cout << "Using dummy water cartoon texture" << std::endl;
            // Create a more cartoon-like texture with blue tones
            waterCartoonTexture = createProceduralCartoonWater();
        }
        else
        {
            std::cout << "Water cartoon texture loaded successfully (alternate path)" << std::endl;
        }
    }
    else
    {
        std::cout << "Water cartoon texture loaded successfully" << std::endl;
    }

    // Add the DUDV map for water distortion effect
    GLuint waterDudvMap = loadTexture("../../../asset/water_asset/dudv.jpg");
    if (waterDudvMap == 0)
    {
        // Try alternate paths
        waterDudvMap = loadTexture("../../../asset/water_asset/water_dudv.png");
        if (waterDudvMap == 0)
        {
            std::cout << "Using procedural DUDV map" << std::endl;
            // Create a procedural DUDV map
            waterDudvMap = createProceduralDudvMap();
        }
        else
        {
            std::cout << "Water DUDV map loaded successfully (alternate path)" << std::endl;
        }
    }
    else
    {
        std::cout << "Water DUDV map loaded successfully" << std::endl;
    }

    // Simple terrain generation with enhanced relief
    for (int i = 0; i <= gridSize; i++)
    {
        for (int j = 0; j <= gridSize; j++)
        {
            float x = (float)j / gridSize * 2.0f - 1.0f;
            float z = (float)i / gridSize * 2.0f - 1.0f;

            float distanceFromCenter = sqrt(x * x + z * z);
            float height = calculateTerrainHeight(x, z);

            vertices.push_back(x);
            vertices.push_back(height);
            vertices.push_back(z);

            vertices.push_back((float)j / gridSize * 3.0f); // Increased texture repetition
            vertices.push_back((float)i / gridSize * 3.0f); // for more detail
            vertices.push_back(height);                     // Store height
        }
    }

    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            int topLeft = i * (gridSize + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = (i + 1) * (gridSize + 1) + j;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    int islandIndicesCount = indices.size();

    // Simplified water plane
    float waterLevel = -0.05f;
    int waterStartIndex = vertices.size() / 6;
    float waterSize = 15.0f;

    const int waterResolution = 100;
    const float waterStep = 2.0f * waterSize / waterResolution;

    float waterTiling = 15.0f; // Increase for more repeats, adjust as you like

    for (int i = 0; i <= waterResolution; i++)
    {
        for (int j = 0; j <= waterResolution; j++)
        {
            float x = -waterSize + j * waterStep;
            float z = -waterSize + i * waterStep - 0.0f;

            vertices.push_back(x);
            vertices.push_back(waterLevel);
            vertices.push_back(z);
            vertices.push_back((float)j / waterResolution * waterTiling); // Tiled UV
            vertices.push_back((float)i / waterResolution * waterTiling); // Tiled UV
            vertices.push_back(1.0f);                                     // Mark as water
        }
    }

    // Create indices for the water grid
    for (int i = 0; i < waterResolution; i++)
    {
        for (int j = 0; j < waterResolution; j++)
        {
            int topLeft = waterStartIndex + i * (waterResolution + 1) + j;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (waterResolution + 1);
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    int waterIndicesCount = waterResolution * waterResolution * 6;

    generateCircle(vertices, indices, 0.1f, 20);

    GLuint vao, vbo, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Use simplified shaders initially
    const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aTexCoord;
        out vec2 TexCoord;
        out vec3 FragPos;
        out float IsWater;
        out vec3 Normal;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform float time;
        
        void main() {
            vec3 pos = aPos;
            
            // Default normal pointing up
            vec3 normal = vec3(0.0, 1.0, 0.0);
            
            // Water surfaces are now flat - no wave animation
            // The IsWater flag is still passed to the fragment shader for different rendering
            
            gl_Position = projection * view * model * vec4(pos, 1.0);
            TexCoord = aTexCoord.xy;
            FragPos = pos;
            IsWater = aTexCoord.z;
            Normal = normal;
        }
    )";

    // Update the fragment shader for more dynamic water appearance
    const char *fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoord;
        in vec3 FragPos;
        in float IsWater;
        in vec3 Normal;
        out vec4 FragColor;
        
        uniform bool isCircle;
        uniform sampler2D sandTexture;
        uniform sampler2D grassTexture;
        uniform sampler2D waterTexture;
        uniform float time;
        uniform sampler2D dudvMap;
        
        // Helper function to estimate terrain normals in the fragment shader
        vec3 estimateNormal(vec3 pos) {
            // Only use this for terrain (not water)
            if (IsWater > 0.5) return Normal;
            
            // Terrain height is stored in FragPos.y
            float height = pos.y;
            
            // Use terrain position to create a normal that varies with position
            // This creates a bumpy effect without actual displacement
            float xSlope = sin(pos.x * 15.0 + pos.z * 13.0) * 0.2;
            float zSlope = cos(pos.z * 17.0 + pos.x * 11.0) * 0.2;
            
            return normalize(vec3(-xSlope, 1.0, -zSlope));
        }
        
        void main() {
            // Get a terrain-specific normal for lighting
            vec3 terrainNormal = IsWater > 0.5 ? Normal : estimateNormal(FragPos);
            
            // Enhanced directional lighting for better 3D look
            vec3 lightDir = normalize(vec3(0.5, 0.8, 0.2));
            float diff = max(dot(terrainNormal, lightDir), 0.3); // Add ambient component
            
            if (isCircle) {
                FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red for circle
            } 
            else if (IsWater > 0.5) {
                // Calculate distance from shore for sand blending
                float distanceFromCenter = length(vec2(FragPos.x, FragPos.z));
                float shoreFactor = smoothstep(0.7, 0.9, distanceFromCenter);
                
                // Create multiple layers of animated DUDV distortion for dynamic water
                // First distortion layer - large scale movement
                vec2 dudvCoord1 = TexCoord * 1.5;
                dudvCoord1.x += time * 0.04;
                dudvCoord1.y -= time * 0.03;
                vec2 dudvValue1 = (texture(dudvMap, dudvCoord1).rg * 2.0 - 1.0) * 0.02;
                
                // Second distortion layer - medium scale detail
                vec2 dudvCoord2 = TexCoord * 3.0 + dudvValue1 * 0.5;
                dudvCoord2.x -= time * 0.02;
                dudvCoord2.y += time * 0.05;
                vec2 dudvValue2 = (texture(dudvMap, dudvCoord2).rg * 2.0 - 1.0) * 0.015;
                
                // Third distortion layer - small ripple details
                vec2 dudvCoord3 = TexCoord * 5.0;
                dudvCoord3.x += time * 0.06 + dudvValue1.y * 2.0;
                dudvCoord3.y += time * 0.04 - dudvValue1.x * 2.0;
                vec2 dudvValue3 = (texture(dudvMap, dudvCoord3).rg * 2.0 - 1.0) * 0.008;
                
                // Combine all distortion layers for complex water movement
                vec2 finalDistortion = dudvValue1 + dudvValue2 + dudvValue3;
                
                // Create dynamic flowing water effect with moving texture + distortion
                vec2 flowingTexCoord = TexCoord;
                flowingTexCoord.x += time * 0.05 + sin(time * 0.2) * 0.01; // Variable speed
                flowingTexCoord.y -= time * 0.03 + cos(time * 0.3) * 0.01;
                
                // Apply the combined distortion to the moving texture coordinates
                vec2 distortedUV = flowingTexCoord * 3.0 + finalDistortion;
                
                // Sample water texture with the complex distortion
                vec4 waterColor = texture(waterTexture, distortedUV);
                
                // Create dynamic specular highlights that move with the water
                float highlight = pow(max(0.0, dot(reflect(-lightDir, 
                               normalize(Normal + vec3(finalDistortion.x, 0.0, finalDistortion.y) * 0.5)), 
                               vec3(0.0, 1.0, 0.0))), 
                               64.0 + sin(time) * 16.0) * 0.5;
                
                // Sample sand texture near shores with subtle movement
                vec2 sandCoord = TexCoord;
                sandCoord.x += time * 0.01 + finalDistortion.x * 0.2;
                sandCoord.y += finalDistortion.y * 0.2;
                vec4 sandColor = texture(sandTexture, sandCoord * 6.0);
                
                // Create base water color with depth gradient
                vec3 deepColor = vec3(0.0, 0.15, 0.3);
                vec3 shallowColor = vec3(0.0, 0.5, 0.8);
                vec3 baseWaterColor = mix(shallowColor, deepColor, shoreFactor);
                
                // Add some subtle color variation based on time
                baseWaterColor += vec3(0.02 * sin(time * 0.3), 0.02 * cos(time * 0.4), 0.03 * sin(time * 0.2));
                
                // Blend water texture details
                vec3 finalWaterColor = mix(baseWaterColor, waterColor.rgb, 0.3);
                
                // Add highlight that moves with the water
                finalWaterColor += vec3(highlight);
                
                // Blend with sand at the edges
                float sandBlend = 0.0;
                if (distanceFromCenter > 0.7 && distanceFromCenter < 0.78) {
                    sandBlend = 1.0 - smoothstep(0.7, 0.78, distanceFromCenter);
                    sandBlend *= 0.7; // Reduce sand intensity
                    
                    // Add some noise to sand edge with subtle animation
                    sandBlend *= (0.8 + 0.4 * sin(FragPos.x * 30.0 + FragPos.z * 25.0 + time * 0.2));
                    
                    finalWaterColor = mix(finalWaterColor, sandColor.rgb, sandBlend);
                }
                
                // Final water opacity - more transparent at edges
                float alpha = mix(0.7, 0.9, shoreFactor);
                
                // Make sand areas more opaque
                alpha = mix(alpha, 0.95, sandBlend);
                
                FragColor = vec4(finalWaterColor, alpha);
            } 
            else {
                // Enhanced terrain rendering with height-based texture blending
                float height = FragPos.y * 5.0; // Scale up for better visibility
                
                // Sample both textures with different scales for more detail
                vec4 sandTex = texture(sandTexture, TexCoord * 3.0);
                vec4 grassTex = texture(grassTexture, TexCoord * 4.0);
                
                // Height-based blending with smooth transition
                float blendFactor = smoothstep(0.0, 0.1, height);
                vec4 terrainColor = mix(sandTex, grassTex, blendFactor);
                
                // Apply directional lighting for better 3D look
                terrainColor.rgb *= diff;
                
                // Add subtle height-based coloring for even more depth
                terrainColor.rgb = mix(terrainColor.rgb, vec3(0.7, 0.7, 0.5), max(0.0, height - 0.2) * 0.5);
                
                FragColor = terrainColor;
            }
        }
    )";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check shader compilation
    if (!checkShaderErrors(vertexShader))
    {
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check shader compilation
    if (!checkShaderErrors(fragmentShader))
    {
        return -1;
    }

    GLuint shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);

    // Check program linking
    if (!checkShaderErrors(shader, true))
    {
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glEnable(GL_DEPTH_TEST);

    sf::Clock clock;
    bool keyPressed = false;

    // After shader compilation, add texture uniforms
    glUseProgram(shader);

    // Print uniform locations to debug
    GLint sandTexLoc = glGetUniformLocation(shader, "sandTexture");
    GLint grassTexLoc = glGetUniformLocation(shader, "grassTexture");
    GLint waterTexLoc = glGetUniformLocation(shader, "waterTexture");

    std::cout << "Uniform locations - sand: " << sandTexLoc
              << ", grass: " << grassTexLoc
              << ", water: " << waterTexLoc << std::endl;

    // Set texture uniforms
    glUniform1i(sandTexLoc, 0);  // Texture unit 0
    glUniform1i(grassTexLoc, 1); // Texture unit 1
    glUniform1i(waterTexLoc, 2); // Texture unit 2

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
            circleZ -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            circleZ += moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            circleX -= moveSpeed;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            circleX += moveSpeed;

        float distanceFromCenter = sqrt(circleX * circleX + circleZ * circleZ);
        const float islandEdge = 0.8f;

        if (distanceFromCenter > islandEdge)
        {
            float angle = atan2(circleZ, circleX);
            circleX = islandEdge * cos(angle);
            circleZ = islandEdge * sin(angle);
        }

        // Use a simple clear color to make sure rendering is happening
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 1.8f, 2.0f), // Fixed camera position
            glm::vec3(0, 0.1, 0),
            glm::vec3(0, 1, 0));

        sf::Vector2u winSize = window.getSize();
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            static_cast<float>(winSize.x) / static_cast<float>(winSize.y),
            0.1f, 100.f);

        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(glGetUniformLocation(shader, "time"), clock.getElapsedTime().asSeconds());

        // Bind textures for water and terrain
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sandTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, grassTexture);

        glActiveTexture(GL_TEXTURE2);
        // Switch to the cartoon texture for the main water appearance
        glBindTexture(GL_TEXTURE_2D, waterCartoonTexture);

        // Make sure isCircle is set correctly before each draw call
        glUniform1i(glGetUniformLocation(shader, "isCircle"), GL_FALSE);
        glDrawElements(GL_TRIANGLES, islandIndicesCount, GL_UNSIGNED_INT, 0);

        // Before drawing water, bind the DUDV map to an additional texture unit for water distortion
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, waterDudvMap);
        // Add a new uniform for the DUDV map if not already present in shader
        glUniform1i(glGetUniformLocation(shader, "dudvMap"), 3);

        // Set blend mode for water
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawElements(GL_TRIANGLES, waterIndicesCount, GL_UNSIGNED_INT,
                       (void *)(islandIndicesCount * sizeof(GLuint)));
        glDisable(GL_BLEND);

        glm::mat4 circleModel = glm::mat4(1.0f);
        circleModel = glm::translate(circleModel, glm::vec3(circleX, 0.0f, circleZ));

        // Calculate the terrain height at the player's position using the same formula as for terrain generation
        float characterHeight = 0.0f;

        if (distanceFromCenter < 0.8f)
            characterHeight = calculateTerrainHeight(circleX, circleZ);

        // Add a small offset to ensure the character stays visibly above terrain
        characterHeight += 0.02f;

        circleModel = glm::translate(circleModel, glm::vec3(0.0f, characterHeight, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(circleModel));
        glUniform1i(glGetUniformLocation(shader, "isCircle"), true);
        glUniform3f(glGetUniformLocation(shader, "circlePos"), circleX, characterHeight, circleZ);

        glDrawElements(GL_TRIANGLES, indices.size() - islandIndicesCount - waterIndicesCount, GL_UNSIGNED_INT,
                       (void *)((islandIndicesCount + waterIndicesCount) * sizeof(GLuint)));

        // For the first frame, check OpenGL errors
        static bool firstFrame = true;
        if (firstFrame)
        {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR)
            {
                std::cout << "OpenGL error: " << err << std::endl;
            }
            firstFrame = false;
        }

        window.display();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteProgram(shader);

    return 0;
}

// Add these texture loading functions at the end of the file
GLuint loadTexture(const std::string &path)
{
    sf::Image img;
    if (!img.loadFromFile(path))
    {
        std::cerr << "Could not load texture: " << path << std::endl;
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}

GLuint createDummyTexture(int r, int g, int b)
{
    const int width = 256, height = 256;
    std::vector<unsigned char> data(width * height * 4);

    for (int i = 0; i < width * height; i++)
    {
        data[i * 4 + 0] = r;   // R
        data[i * 4 + 1] = g;   // G
        data[i * 4 + 2] = b;   // B
        data[i * 4 + 3] = 255; // A
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return tex;
}
