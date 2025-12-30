/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Agent.cpp - Main AI agent implementation
*/

#include "Agent.hpp"
#include <chrono>
#include <iostream>
#include <regex>
#include <random>

/**
 * @brief Constructs a new AI agent
 * 
 * @param host Server hostname
 * @param port Server port
 * @param team Team name for the agent
 */
Agent::Agent(const std::string &host,
             const std::string &port,
             const std::string &team)
    : sock(host, port), parser(state)
{
    std::cout << "[DEBUG] Agent: Initializing agent for team '" << team << "'" << std::endl;
    waitWelcome();
    std::cout << "[DEBUG] Agent: Sending team name: " << team << std::endl;
    sock.sendLine(team);
}

/**
 * @brief Main agent loop that handles decision making and command execution
 * 
 * The agent continuously:
 * - Reads server responses
 * - Checks if it's in incantation mode (frozen state)
 * - Gets commands from the planner
 * - Queues commands for execution
 * - Sends commands to the server
 * - Handles emergency situations (getting stuck)
 * - Ensures organic movement patterns
 */
void Agent::run()
{
    int consecutiveForwards = 0;
    int consecutiveLefts = 0;
    int consecutiveRights = 0;
    std::string lastMovementCommand = "";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> leftOrRight(0, 1);

    while (true)
    {
        readServer();

        auto elevationStrategy = dynamic_cast<ElevationStrategy*>(planner.getStrategies()[0].get());
        if (elevationStrategy && elevationStrategy->getIsInIncantation())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        auto cmds = planner.decide(state);
        std::cout << "[DEBUG] Agent: Planner decided on " << cmds.size() << " commands" << std::endl;

        // If any command is 'Take food', clear the queue to prioritize food collection
        bool hasTakeFood = false;
        for (const auto &cmd : cmds) {
            if (cmd == "Take food") {
                hasTakeFood = true;
                break;
            }
        }
        if (hasTakeFood) {
            std::cout << "[DEBUG] Agent: Clearing command queue to prioritize Take food" << std::endl;
            queue.clear();
        }

        for (const auto &cmd : cmds)
        {
            if (cmd == "Look")
            {
                queue.push(cmd);
            }
            else if (queue.size() < 15)
            {
                queue.push(cmd);
            }
        }

        static int samePositionCount = 0;
        static int lastPosX = -1, lastPosY = -1;

        if (state.posX == lastPosX && state.posY == lastPosY)
        {
            samePositionCount++;
            if (samePositionCount > 5)
            {
                std::cout << "[DEBUG] Agent: Stuck in same position for " << samePositionCount << " turns, clearing queue and forcing Forward" << std::endl;
                queue.clear();
                samePositionCount = 0;
                queue.push("Forward");
            }
        }
        else
        {
            samePositionCount = 0;
            lastPosX = state.posX;
            lastPosY = state.posY;
        }

        if (const std::string *cmd = queue.popReady())
        {
            std::cout << "[DEBUG] Agent: Sending command: " << *cmd << std::endl;
            sock.sendLine(*cmd);
            lastSentCommand = *cmd;

            if (*cmd == "Incantation")
            {
                planner.setIncantationSent();
            }

            // Track consecutive movements for organic behavior
            if (*cmd == "Forward")
            {
                if (lastMovementCommand == "Forward")
                {
                    consecutiveForwards++;
                    if (consecutiveForwards >= 3)
                    {
                        std::string forced = leftOrRight(gen) == 0 ? "Left" : "Right";
                        std::cout << "[DEBUG] Agent: Forced direction change after 3 Forwards: " << forced << std::endl;
                        queue.pushFront(forced);
                        consecutiveForwards = 0;
                        consecutiveLefts = 0;
                        consecutiveRights = 0;
                    }
                }
                else
                {
                    consecutiveForwards = 1;
                    consecutiveLefts = 0;
                    consecutiveRights = 0;
                }
                lastMovementCommand = "Forward";
                queue.push("Look");
            }
            else if (*cmd == "Left")
            {
                if (lastMovementCommand == "Left")
                {
                    consecutiveLefts++;
                    if (consecutiveLefts >= 3)
                    {
                        std::string forced = leftOrRight(gen) == 0 ? "Forward" : "Right";
                        std::cout << "[DEBUG] Agent: Forced direction change after 3 Lefts: " << forced << std::endl;
                        queue.pushFront(forced);
                        consecutiveForwards = 0;
                        consecutiveLefts = 0;
                        consecutiveRights = 0;
                    }
                }
                else
                {
                    consecutiveLefts = 1;
                    consecutiveForwards = 0;
                    consecutiveRights = 0;
                }
                lastMovementCommand = "Left";
            }
            else if (*cmd == "Right")
            {
                if (lastMovementCommand == "Right")
                {
                    consecutiveRights++;
                    if (consecutiveRights >= 3)
                    {
                        std::string forced = leftOrRight(gen) == 0 ? "Forward" : "Left";
                        std::cout << "[DEBUG] Agent: Forced direction change after 3 Rights: " << forced << std::endl;
                        queue.pushFront(forced);
                        consecutiveForwards = 0;
                        consecutiveLefts = 0;
                        consecutiveRights = 0;
                    }
                }
                else
                {
                    consecutiveRights = 1;
                    consecutiveForwards = 0;
                    consecutiveLefts = 0;
                }
                lastMovementCommand = "Right";
            }
            else
            {
                consecutiveForwards = 0;
                consecutiveLefts = 0;
                consecutiveRights = 0;
                lastMovementCommand = "";
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

/**
 * @brief Waits for the server welcome message
 */
void Agent::waitWelcome()
{
    std::cout << "[DEBUG] Agent: Waiting for server welcome message..." << std::endl;
    while (true)
    {
        for (auto &l : sock.recvLines(100))
        {
            std::cout << "[DEBUG] Agent: Received during welcome wait: " << l << std::endl;
            if (l == "WELCOME")
            {
                std::cout << "[DEBUG] Agent: Welcome message received, proceeding with initialization" << std::endl;
                return;
            }
        }
    }
}

/**
 * @brief Reads and processes server responses
 * 
 * Handles various server messages:
 * - Command acknowledgments (ok/ko)
 * - Elevation responses
 * - Broadcast messages
 * - World size information
 * - Death notification
 */
void Agent::readServer()
{
    for (auto &line : sock.recvLines())
    {
        std::cout << "[DEBUG] Agent: Processing server response: '" << line << "'" << std::endl;
        
        if (line == "ok" || line == "ko")
        {
            std::cout << "[DEBUG] Agent: Command acknowledgment: " << line << " for last command: " << lastSentCommand << std::endl;
            planner.setLastResult(line);
            parser.onCommandAck(lastSentCommand);
            queue.ack();
            continue;
        }
        if (line == "dead")
        {
            std::cout << "[DEBUG] Agent: DEATH NOTIFICATION received!" << std::endl;
            throw std::runtime_error("bot died");
        }

        if (line == "Elevation underway")
        {
            std::cout << "[DEBUG] Agent: Elevation started" << std::endl;
            planner.setLastResult(line);
            parser.ingest(line);
            continue;
        }

        std::regex broadcastRegex(R"(message\s+(\d+),\s*(.+))");
        std::smatch broadcastMatch;
        if (std::regex_match(line, broadcastMatch, broadcastRegex))
        {
            std::string message = broadcastMatch[2];
            std::cout << "[DEBUG] Agent: Broadcast message from player " << broadcastMatch[1] << ": " << message << std::endl;
            planner.handleBroadcastMessage(message);
            parser.ingest(line);
            continue;
        }

        parser.ingest(line);

        std::regex worldSizeRegex(R"((\d+)\s+(\d+))");
        std::smatch m;
        if (std::regex_match(line, m, worldSizeRegex))
        {
            int width = std::stoi(m[1]);
            int height = std::stoi(m[2]);
            std::cout << "[DEBUG] Agent: World size received: " << width << "x" << height << std::endl;
            planner.setWorldSize(width, height);
        }
    }
}
