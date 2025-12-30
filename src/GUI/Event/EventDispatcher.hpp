/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** EventDispatcher.hpp
*/

#pragma once
#include <vector>
#include <memory>
#include "IEventListener.hpp"

class Event;

class EventDispatcher
{
public:
    void addListener(std::shared_ptr<IEventListener> listener)
    {
        listeners.push_back(listener);
    }

    void dispatch(const Event &event)
    {
        for (auto &listener : listeners)
            listener->onEvent(event);
    }

private:
    std::vector<std::shared_ptr<IEventListener>> listeners;
};
