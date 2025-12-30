/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Event.hpp
*/

#pragma once

struct Event
{
    enum Type
    {
        WindowClosed,
        KeyPressed
    } type;
};
