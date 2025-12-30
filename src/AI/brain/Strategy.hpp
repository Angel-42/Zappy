/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Strategy.hpp
*/

#pragma once
#include "../WorldState.hpp"
#include <vector>
#include <string>

class Strategy
{
public:
    virtual ~Strategy() = default;

    /** Whether this strategy wants to act, given the latest world state. */
    virtual bool applicable(const WorldState &st) const = 0;

    /**
     * Produce a *small* list of commands (≤ 3 is good),
     * ordered exactly as they should be queued.
     */
    virtual std::vector<std::string> getCommands(const WorldState &st) = 0;
};
