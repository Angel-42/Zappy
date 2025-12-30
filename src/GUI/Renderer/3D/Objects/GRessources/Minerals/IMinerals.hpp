/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IMinerals.hpp
*/

#pragma once

#include "../IRessources.hpp"

class IMinerals : public IRessources
{
public:
    virtual ~IMinerals() = default;
    virtual void getMineralType() const = 0;
};
