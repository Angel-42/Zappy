/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IEventListener.hpp
*/

#pragma once

class Event;

class IEventListener
{
public:
    virtual ~IEventListener() = default;
    virtual void onEvent(const Event &event) = 0;
};
