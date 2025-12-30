/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** TextureManager.hpp
*/

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <GL/glew.h>

class TextureManager
{
public:
    TextureManager() = default;
    ~TextureManager();

    [[nodiscard]] GLuint load(std::string_view path);
    void remove(std::string_view path);
    void clear();

private:
    std::unordered_map<std::string, GLuint> m_textures;
};
