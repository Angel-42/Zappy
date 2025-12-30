/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** StateMachine.hpp
*/

#pragma once

#include "IState.hpp"
#include <memory>
#include <stack>
#include <functional>

class StateMachine
{
public:
    StateMachine() = default;

    void pushState(std::unique_ptr<IState> state)
    {
        if (!states.empty())
            states.top()->onExit(*this);
        states.push(std::move(state));
        states.top()->onEnter(*this);
    }

    void changeState(std::unique_ptr<IState> state)
    {
        if (!states.empty())
        {
            states.top()->onExit(*this);
            states.pop();
        }
        states.push(std::move(state));
        states.top()->onEnter(*this);
    }

    void popState()
    {
        if (!states.empty())
        {
            states.top()->onExit(*this);
            states.pop();
        }
        if (!states.empty())
            states.top()->onEnter(*this);
    }

    [[nodiscard]] IState *currentState()
    {
        return states.empty() ? nullptr : states.top().get();
    }

    void update(float deltaTime)
    {
        if (auto *state = currentState())
            state->update(*this, deltaTime);
    }

    void render()
    {
        if (auto *state = currentState())
            state->render(*this);
    }

    void handleEvent(const sf::Event &event)
    {
        if (auto *state = currentState())
            state->handleEvent(*this, event);
    }

    [[nodiscard]] bool empty() const { return states.empty(); }

    void clearStates()
    {
        while (!states.empty())
        {
            states.top()->onExit(*this);
            states.pop();
        }
    }

private:
    std::stack<std::unique_ptr<IState>> states;
};
