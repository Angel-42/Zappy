/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ShaderFileLoader.hpp
*/

#pragma once

#include <string>

class ShaderFileLoader
{
public:
    static std::string load(const std::string &path);

private:
    static bool hasValidExtension(const std::string &path);
};
