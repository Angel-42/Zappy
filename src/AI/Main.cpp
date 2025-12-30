/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Main.cpp
*/

#include "Agent.hpp"
#include <iostream>
#include <cstring>

int main(int argc, char **argv)
{
    std::string host = "127.0.0.1";
    std::string port;
    std::string team;

    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-p") == 0 && i + 1 < argc)
        {
            port = argv[++i];
        }
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
        {
            team = argv[++i];
        }
        else if (strcmp(argv[i], "-h") == 0 && i + 1 < argc)
        {
            host = argv[++i];
        }
    }

    if (strcmp(team.c_str(), "GRAPHIC") == 0)
    {
        std::cerr << "Error: Cannot use team name 'GRAPHIC' for AI bots.\n";
        return 1;
    }

    if (port.empty() || team.empty())
    {
        std::cerr << "USAGE: ./zappy_ai -p port -n name -h machine\n";
        return 1;
    }

    try
    {
        Agent bot(host.c_str(), port.c_str(), team.c_str());
        bot.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
