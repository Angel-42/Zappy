/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ShaderManager.cpp
*/

#include "ShaderManager.hpp"
#include "../../Utils/ShaderFileLoader.hpp"

bool ShaderManager::load(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertSrc = ShaderFileLoader::load(vertexPath);
    std::string fragSrc = ShaderFileLoader::load(fragmentPath);

    if (vertSrc.empty() || fragSrc.empty())
        return false;

    auto shader = std::make_unique<GLShader>(vertSrc.c_str(), fragSrc.c_str());

    m_shaders[name] = std::move(shader);
    return true;
}

GLShader *ShaderManager::get(const std::string &name)
{
    auto it = m_shaders.find(name);
    if (it != m_shaders.end())
        return it->second.get();
    return nullptr;
}

void ShaderManager::remove(const std::string &name)
{
    m_shaders.erase(name);
}

void ShaderManager::clear()
{
    m_shaders.clear();
}
