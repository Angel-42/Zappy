/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** TextureManager.cpp
*/

#include "TextureManager.hpp"
#include <SFML/Graphics/Image.hpp>
#include <iostream>
#include <string_view>

TextureManager::~TextureManager() { clear(); }

GLuint TextureManager::load(std::string_view path)
{
    if (auto it = m_textures.find(std::string(path)); it != m_textures.end())
        return it->second;

    sf::Image img;
    if (!img.loadFromFile(std::string(path)))
    {
        std::cerr << "TextureManager: Failed to load " << path << std::endl;
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

    m_textures.emplace(std::string(path), tex);
    return tex;
}

void TextureManager::remove(std::string_view path)
{
    if (auto it = m_textures.find(std::string(path)); it != m_textures.end())
    {
        glDeleteTextures(1, &it->second);
        m_textures.erase(it);
    }
}

void TextureManager::clear()
{
    for (auto &[key, tex] : m_textures)
        glDeleteTextures(1, &tex);
    m_textures.clear();
}
