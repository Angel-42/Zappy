/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ShaderManager.hpp
*/

#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "../../Utils/GLShader.hpp"

class ShaderManager
{
public:
    ShaderManager() = default;
    ~ShaderManager() = default;

    [[nodiscard]] bool load(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath);
    [[nodiscard]] GLShader *get(const std::string &name);
    void remove(const std::string &name);
    void clear();

private:
    std::unordered_map<std::string, std::unique_ptr<GLShader>> m_shaders;
};
