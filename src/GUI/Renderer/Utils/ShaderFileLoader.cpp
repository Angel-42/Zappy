/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ShaderFileLoader.cpp
*/

#include "ShaderFileLoader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>

static const std::vector<std::string> shaderExtensions = {
    ".vert", ".frag", ".glsl", ".geom"};

bool ShaderFileLoader::hasValidExtension(const std::string &path)
{
    auto dot = path.find_last_of('.');
    if (dot == std::string::npos)
        return false;
    std::string ext = path.substr(dot);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return std::find(shaderExtensions.begin(), shaderExtensions.end(), ext) != shaderExtensions.end();
}

std::string ShaderFileLoader::load(const std::string &path)
{
    if (!hasValidExtension(path))
        throw std::runtime_error("File does not have a recognized shader extension: " + path);

    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file)
        throw std::runtime_error("Failed to open file: " + path);

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string str = buffer.str();

    // Remove UTF-8 BOM if present
    if (str.size() >= 3 &&
        static_cast<unsigned char>(str[0]) == 0xEF &&
        static_cast<unsigned char>(str[1]) == 0xBB &&
        static_cast<unsigned char>(str[2]) == 0xBF)
    {
        str = str.substr(3);
    }

    if (str.empty())
        throw std::runtime_error("File is empty: " + path);

    return str;
}
