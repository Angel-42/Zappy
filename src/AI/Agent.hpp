/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Agent.hpp
*/

#pragma once
#include "Socket.hpp"
#include "CommandQueue.hpp"
#include "Parser.hpp"
#include "brain/Planner.hpp"
#include <thread>
#include <iostream>

class Agent
{
public:
    Agent(const std::string &host,
          const std::string &port,
          const std::string &team);

    void run();

private:
    void waitWelcome();
    void readServer();

    Socket sock;
    CommandQueue queue;
    WorldState state;
    Parser parser;
    Planner planner;

    std::string lastSentCommand;
};
