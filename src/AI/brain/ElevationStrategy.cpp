/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ElevationStrategy.cpp - Player elevation (incantation) logic implementation
*/

#include "ElevationStrategy.hpp"

ElevationStrategy::ElevationStrategy() : lastBroadcastTime(0), broadcastCooldown(10) {}

// Define elevation requirements according to Zappy documentation
const std::unordered_map<int, ElevationRequirements> ElevationStrategy::elevationRules = {
    {1, {1, 1, 0, 0, 0, 0, 0}},      // 1->2: 1 player, 1 linemate
    {2, {2, 1, 1, 1, 0, 0, 0}},      // 2->3: 2 players, 1 linemate, 1 deraumere, 1 sibur
    {3, {2, 2, 0, 1, 0, 2, 0}},      // 3->4: 2 players, 2 linemate, 1 sibur, 2 phiras
    {4, {4, 1, 1, 2, 0, 1, 0}},      // 4->5: 4 players, 1 linemate, 1 deraumere, 2 sibur, 1 phiras
    {5, {4, 1, 2, 1, 3, 0, 0}},      // 5->6: 4 players, 1 linemate, 2 deraumere, 1 sibur, 3 mendiane
    {6, {6, 1, 2, 3, 0, 1, 0}},      // 6->7: 6 players, 1 linemate, 2 deraumere, 3 sibur, 1 phiras
    {7, {6, 2, 2, 2, 2, 2, 1}}       // 7->8: 6 players, 2 of each mineral
}; 

bool ElevationStrategy::applicable(const WorldState &st) const
{
    std::cout << "[DEBUG] ElevationStrategy: Checking if applicable" << std::endl;
    
    // Always check for food anywhere in vision first - don't take precedence if food is available
    if (!st.vision.empty()) {
        // Check current tile
        for (const auto &item : st.vision[0]) {
            if (item == "food") {
                std::cout << "[DEBUG] ElevationStrategy: Food available on current tile, not taking precedence" << std::endl;
                return false;
            }
        }
        
        // Check nearby tiles for food
        for (size_t i = 1; i < st.vision.size(); ++i) {
            for (const auto &item : st.vision[i]) {
                if (item == "food") {
                    std::cout << "[DEBUG] ElevationStrategy: Food available in vision at index " << i << ", not taking precedence" << std::endl;
                    return false;
                }
            }
        }
    }
    
    if (canElevate(st) && !isInIncantation && !incantationQueued) {
        std::cout << "[DEBUG] ElevationStrategy: Can elevate, taking precedence" << std::endl;
        return true;
    }
    if (incantationQueued) {
        std::cout << "[DEBUG] ElevationStrategy: Incantation queued, not applicable" << std::endl;
        return false;
    }
    
    bool result = st.food >= 5 && !isInIncantation;
    std::cout << "[DEBUG] ElevationStrategy: Food >= 5 and not in incantation: " << (result ? "true" : "false") << std::endl;
    return result;
}

std::vector<std::string> ElevationStrategy::getCommands(const WorldState &st)
{
    std::cout << "[DEBUG] ElevationStrategy: getCommands called" << std::endl;
    
    if (canElevate(st) && !incantationQueued)
    {
        std::cout << "[DEBUG] ElevationStrategy: Can elevate, queuing incantation" << std::endl;
        incantationQueued = true;
        return {"Incantation"};
    }

    if (canElevate(st) && incantationQueued)
    {
        std::cout << "[DEBUG] ElevationStrategy: Incantation already queued, waiting" << std::endl;
        return {};
    }

    auto missingResources = getMissingResources(st);
    if (!missingResources.empty())
    {
        std::cout << "[DEBUG] ElevationStrategy: Missing resources: ";
        for (const auto& resource : missingResources) {
            std::cout << resource << " ";
        }
        std::cout << std::endl;
        return collectMissingResources(st, missingResources);
    }

    if (hasAllResourcesForElevation(st))
    {
        std::cout << "[DEBUG] ElevationStrategy: Has all resources, coordinating with team" << std::endl;
        return coordinateWithTeam(st);
    }

    std::cout << "[DEBUG] ElevationStrategy: Exploring for resources" << std::endl;
    return exploreForResources(st);
}

void ElevationStrategy::setIncantationResult(const std::string &result)
{
    isInIncantation = false;
    incantationQueued = false;
    if (result == "Elevation underway")
    {
        lastElevationTime = 0;
    }
    else if (result == "ko")
    {
        elevationFailureCount++;
    }
}

void ElevationStrategy::setIncantationSent()
{
    isInIncantation = true;
}

void ElevationStrategy::setBroadcastMessage(const std::string &message)
{
    if (message.find("NEED_ELEVATION") != std::string::npos)
    {
        teammateNeedsElevation = true;
    }
    else if (message.find("READY_ELEVATION") != std::string::npos)
    {
        teammateReadyForElevation = true;
    }
}

bool ElevationStrategy::getIsInIncantation() const
{
    return isInIncantation;
}

bool ElevationStrategy::canElevate(const WorldState &st) const
{
    auto requirements = getElevationRequirements(st.level);
    if (!requirements)
        return false;

    return st.linemate >= requirements->linemate &&
           st.deraumere >= requirements->deraumere &&
           st.sibur >= requirements->sibur &&
           st.mendiane >= requirements->mendiane &&
           st.phiras >= requirements->phiras &&
           st.thystame >= requirements->thystame;
}

bool ElevationStrategy::hasAllResourcesForElevation(const WorldState &st) const
{
    auto requirements = getElevationRequirements(st.level);
    if (!requirements)
        return false;

    return st.linemate >= requirements->linemate &&
           st.deraumere >= requirements->deraumere &&
           st.sibur >= requirements->sibur &&
           st.mendiane >= requirements->mendiane &&
           st.phiras >= requirements->phiras &&
           st.thystame >= requirements->thystame;
}

std::vector<std::string> ElevationStrategy::getMissingResources(const WorldState &st) const
{
    std::vector<std::string> missing;
    auto requirements = getElevationRequirements(st.level);
    
    if (!requirements)
        return missing;

    if (st.linemate < requirements->linemate)
        missing.push_back("linemate");
    if (st.deraumere < requirements->deraumere)
        missing.push_back("deraumere");
    if (st.sibur < requirements->sibur)
        missing.push_back("sibur");
    if (st.mendiane < requirements->mendiane)
        missing.push_back("mendiane");
    if (st.phiras < requirements->phiras)
        missing.push_back("phiras");
    if (st.thystame < requirements->thystame)
        missing.push_back("thystame");

    return missing;
}

std::vector<std::string> ElevationStrategy::collectMissingResources(const WorldState &st, const std::vector<std::string> &missingResources)
{
    std::vector<std::string> commands;

    std::cout << "[DEBUG] ElevationStrategy: collectMissingResources - looking for: ";
    for (const auto& resource : missingResources) {
        std::cout << resource << " ";
    }
    std::cout << std::endl;

    for (const auto &resource : missingResources)
    {
        std::cout << "[DEBUG] ElevationStrategy: Checking for resource: " << resource << std::endl;
        
        if (!st.vision.empty())
        {
            std::cout << "[DEBUG] ElevationStrategy: Current tile contents: ";
            for (const auto &item : st.vision[0]) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
            
            for (const auto &item : st.vision[0])
            {
                if (item == resource)
                {
                    std::cout << "[DEBUG] ElevationStrategy: Found " << resource << " on current tile" << std::endl;
                    commands.push_back("Take " + resource);
                    commands.push_back("Inventory");
                    return commands;
                }
            }
        }

        std::cout << "[DEBUG] ElevationStrategy: Checking nearby tiles for " << resource << std::endl;
        for (size_t i = 1; i < st.vision.size(); ++i)
        {
            for (const auto &item : st.vision[i])
            {
                if (item == resource)
                {
                    std::cout << "[DEBUG] ElevationStrategy: Found " << resource << " in vision at index " << i << ", moving Forward" << std::endl;
                    return {"Forward", "Look"};
                }
            }
        }
    }

    std::cout << "[DEBUG] ElevationStrategy: No missing resources found, just looking" << std::endl;
    return {"Look"};
}

std::vector<std::string> ElevationStrategy::coordinateWithTeam(const WorldState &st)
{
    std::vector<std::string> commands;

    if (teammateNeedsElevation)
    {
        commands.push_back("Broadcast READY_ELEVATION");
        teammateNeedsElevation = false;
    }
    else if (teammateReadyForElevation)
    {
        commands.push_back("Broadcast NEED_ELEVATION");
        teammateReadyForElevation = false;
    }
    else
    {
        commands.push_back("Broadcast NEED_ELEVATION");
    }

    return commands;
}

std::vector<std::string> ElevationStrategy::exploreForResources(const WorldState &st)
{
    std::vector<std::string> commands;

    if (!st.vision.empty())
    {
        for (const auto &item : st.vision[0])
        {
            if (item == "food")
            {
                commands.push_back("Take food");
            }
            else if (item == "linemate" || item == "deraumere" || item == "sibur" ||
                     item == "mendiane" || item == "phiras" || item == "thystame")
            {
                commands.push_back("Take " + item);
            }
        }
    }

    if (!commands.empty())
    {
        commands.push_back("Inventory");
        return commands;
    }

    return {"Look"};
}

const ElevationRequirements* ElevationStrategy::getElevationRequirements(int level) const
{
    auto it = elevationRules.find(level);
    if (it != elevationRules.end())
    {
        return &it->second;
    }
    return nullptr;
} 