/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** SurviveStrategy.cpp - Basic survival and resource collection implementation
*/

#include "SurviveStrategy.hpp"

std::function<std::string(const WorldState&, const std::vector<std::pair<int, int>>&)> SurviveStrategy::bfsNavigate = nullptr;

SurviveStrategy::Direction SurviveStrategy::visionIndexToDirection(size_t idx)
{
    switch (idx)
    {
    case 0: return Direction::HERE;
    case 1: return Direction::FORWARD;
    case 2: return Direction::FORWARD_LEFT;
    case 3: return Direction::FORWARD_RIGHT;
    case 4: return Direction::LEFT;
    case 5: return Direction::RIGHT;
    case 6: return Direction::BACK;
    default: return Direction::UNKNOWN;
    }
}

bool SurviveStrategy::applicable(const WorldState &) const
{
    return true;
}

std::vector<std::string> SurviveStrategy::getCommands(const WorldState &st)
{
    std::vector<std::string> commands;

    std::cout << "[DEBUG] SurviveStrategy: Current food level: " << st.food << std::endl;

    if (st.food > 10) {
        std::cout << "[DEBUG] SurviveStrategy: Food > 10, focusing solely on mineral collection" << std::endl;
        return handleMineralCollection(st, true);
    }

    // Always check for food on current tile first, regardless of food level
    if (!st.vision.empty()) {
        std::cout << "[DEBUG] SurviveStrategy: Checking current tile for food first" << std::endl;
        for (const auto &item : st.vision[0]) {
            if (item == "food") {
                std::cout << "[DEBUG] SurviveStrategy: Found food on current tile, prioritizing food collection" << std::endl;
                return {"Take food", "Inventory"};
            }
        }
    }
    
    if (st.food <= 10) {  // Increased threshold from 5 to 10
        std::cout << "[DEBUG] SurviveStrategy: Food is low, prioritizing food collection" << std::endl;
        return handleLowFood(st);
    }

    std::cout << "[DEBUG] SurviveStrategy: Food is sufficient, collecting minerals" << std::endl;
    return handleMineralCollection(st, /*ignoreFood=*/false);
}

std::vector<std::string> SurviveStrategy::handleLowFood(const WorldState &st)
{
    std::vector<std::string> commands;

    std::cout << "[DEBUG] SurviveStrategy: handleLowFood - checking current tile for food" << std::endl;
    
    // Add debug output to show complete vision array
    std::cout << "[DEBUG] SurviveStrategy: Complete vision array (" << st.vision.size() << " tiles):" << std::endl;
    for (size_t i = 0; i < st.vision.size(); ++i) {
        std::cout << "[DEBUG] SurviveStrategy: Tile " << i << ": ";
        for (const auto &item : st.vision[i]) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
    
    // First, check if there's food on the current tile - this takes highest priority
    if (!st.vision.empty()) {
        std::cout << "[DEBUG] SurviveStrategy: Current tile contents: ";
        for (const auto &item : st.vision[0]) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
        
        for (const auto &item : st.vision[0]) {
            if (item == "food") {
                std::cout << "[DEBUG] SurviveStrategy: Found food on current tile, adding Take food command" << std::endl;
                commands.push_back("Take food");
                commands.push_back("Inventory");
                return commands; // Return immediately if food found on current tile
            }
        }
    }

    // If no food on current tile, check nearby tiles for food
    std::cout << "[DEBUG] SurviveStrategy: No food on current tile, checking nearby tiles" << std::endl;
    
    // When food is critically low (below 5), be more aggressive about finding food
    if (st.food < 5) {
        std::cout << "[DEBUG] SurviveStrategy: Food critically low (" << st.food << "), exploring all directions for food" << std::endl;
        
        // Check all vision directions for food
        for (size_t i = 1; i < st.vision.size(); ++i) {
            for (const auto &item : st.vision[i]) {
                if (item == "food") {
                    std::cout << "[DEBUG] SurviveStrategy: Found food in vision at index " << i << ", moving toward it" << std::endl;
                    
                    // Determine the best direction to move based on vision index
                    if (i == 1) {
                        return {"Forward", "Look"};
                    } else if (i == 2) {
                        return {"Left", "Forward", "Look"};
                    } else if (i == 3) {
                        return {"Right", "Forward", "Look"};
                    } else if (i == 4) {
                        return {"Left", "Look"};
                    } else if (i == 5) {
                        return {"Right", "Look"};
                    } else if (i == 6) {
                        return {"Right", "Right", "Forward", "Look"};
                    } else if (i == 7) {
                        return {"Left", "Left", "Forward", "Look"};
                    } else if (i == 8) {
                        return {"Right", "Right", "Forward", "Look"};
                    }
                }
            }
        }
        
        // If no food found in vision, try systematic exploration
        std::cout << "[DEBUG] SurviveStrategy: No food in vision, trying systematic exploration" << std::endl;
        static int explorationStep = 0;
        explorationStep = (explorationStep + 1) % 4;
        
        switch (explorationStep) {
            case 0:
                std::cout << "[DEBUG] SurviveStrategy: Exploration step 0: Forward" << std::endl;
                return {"Forward", "Look"};
            case 1:
                std::cout << "[DEBUG] SurviveStrategy: Exploration step 1: Left" << std::endl;
                return {"Left", "Look"};
            case 2:
                std::cout << "[DEBUG] SurviveStrategy: Exploration step 2: Right" << std::endl;
                return {"Right", "Look"};
            case 3:
                std::cout << "[DEBUG] SurviveStrategy: Exploration step 3: Forward" << std::endl;
                return {"Forward", "Look"};
        }
    } else {
        // Normal food search for food levels 5-10
        for (size_t i = 1; i < st.vision.size(); ++i) {
            for (const auto &item : st.vision[i]) {
                if (item == "food") {
                    std::cout << "[DEBUG] SurviveStrategy: Found food in vision at index " << i << ", moving Forward" << std::endl;
                    return {"Forward", "Look"};
                }
            }
        }
    }

    // Only if no food found anywhere, then collect minerals from current tile
    if (!st.vision.empty()) {
        for (const auto &item : st.vision[0]) {
            if (isMineral(item)) {
                std::cout << "[DEBUG] SurviveStrategy: No food found, collecting mineral " << item << " from current tile" << std::endl;
                commands.push_back("Take " + item);
            }
        }
    }

    if (!commands.empty()) {
        std::cout << "[DEBUG] SurviveStrategy: Adding Inventory command after collection commands" << std::endl;
        commands.push_back("Inventory");
        return commands;
    }

    auto foodTiles = st.mapMemory.findResource("food");
    if (!foodTiles.empty() && bfsNavigate) {
        std::cout << "[DEBUG] SurviveStrategy: Using BFS navigation to find food" << std::endl;
        std::string navCmd = bfsNavigate(st, foodTiles);
        if (!navCmd.empty()) {
            return {navCmd, "Look"};
        }
    }

    std::cout << "[DEBUG] SurviveStrategy: No food found, just looking around" << std::endl;
    return {"Look"};
}

std::vector<std::string> SurviveStrategy::handleMineralCollection(const WorldState &st, bool ignoreFood)
{
    std::vector<std::string> minerals = {"linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};
    std::cout << "[DEBUG] SurviveStrategy: handleMineralCollection - checking mineral needs (ignoreFood=" << ignoreFood << ")" << std::endl;

    for (const auto &mineral : minerals) {
        int invCount = 0;
        auto it = st.inventory.find(mineral);
        if (it != st.inventory.end()) invCount = it->second;

        std::cout << "[DEBUG] SurviveStrategy: Mineral " << mineral << " count: " << invCount << std::endl;
        if (invCount < 1) {
            std::cout << "[DEBUG] SurviveStrategy: Need " << mineral << ", trying to collect it" << std::endl;
            auto commands = collectMineral(st, mineral);
            if (!commands.empty()) {
                std::cout << "[DEBUG] SurviveStrategy: Returning mineral collection commands for " << mineral << std::endl;
                return commands;
            }
        }
    }

    // Only collect food if not ignoring it
    if (!ignoreFood && !st.vision.empty()) {
        for (const auto &item : st.vision[0]) {
            if (item == "food") {
                std::cout << "[DEBUG] SurviveStrategy: Found food on current tile during mineral collection, adding Take food command" << std::endl;
                return {"Take food", "Inventory"};
            }
        }
    }

    std::cout << "[DEBUG] SurviveStrategy: All minerals satisfied, falling back to exploration" << std::endl;
    return fallbackExploration(st);
}

std::vector<std::string> SurviveStrategy::collectMineral(const WorldState &st, const std::string &mineral)
{
    std::cout << "[DEBUG] SurviveStrategy: collectMineral - looking for " << mineral << std::endl;
    
    if (!st.vision.empty()) {
        std::cout << "[DEBUG] SurviveStrategy: Checking current tile for " << mineral << std::endl;
        for (const auto &item : st.vision[0]) {
            if (item == mineral) {
                std::cout << "[DEBUG] SurviveStrategy: Found " << mineral << " on current tile" << std::endl;
                return {"Take " + mineral, "Inventory"};
            }
        }
    }

    std::cout << "[DEBUG] SurviveStrategy: Checking nearby tiles for " << mineral << std::endl;
    for (size_t i = 1; i < st.vision.size(); ++i) {
        for (const auto &item : st.vision[i]) {
            if (item == mineral) {
                std::cout << "[DEBUG] SurviveStrategy: Found " << mineral << " in vision at index " << i << ", moving Forward" << std::endl;
                return {"Forward", "Look"};
            }
        }
    }

    auto mineralTiles = st.mapMemory.findResource(mineral);
    if (!mineralTiles.empty() && bfsNavigate) {
        std::cout << "[DEBUG] SurviveStrategy: Using BFS navigation to find " << mineral << std::endl;
        std::string navCmd = bfsNavigate(st, mineralTiles);
        if (!navCmd.empty()) {
            return {navCmd, "Look"};
        }
    }

    std::cout << "[DEBUG] SurviveStrategy: Could not find " << mineral << std::endl;
    return {};
}

std::vector<std::string> SurviveStrategy::fallbackExploration(const WorldState &st)
{
    std::vector<std::string> commands;

    std::cout << "[DEBUG] SurviveStrategy: fallbackExploration - checking current tile for any resources" << std::endl;

    if (st.vision.empty()) {
        std::cout << "[DEBUG] SurviveStrategy: No vision data, just looking" << std::endl;
        return {"Look"};
    }

    // Try to explore unvisited tiles using BFS navigation (Planner will pick target)
    if (bfsNavigate) {
        std::cout << "[DEBUG] SurviveStrategy: Navigating to unvisited tile (Planner will pick target)" << std::endl;
        std::string navCmd = bfsNavigate(st, {});
        if (!navCmd.empty()) {
            return {navCmd, "Look"};
        }
    }

    std::vector<std::string> minerals = {"linemate", "deraumere", "sibur", "mendiane", "phiras", "thystame"};

    // Always prioritize food collection first, even if we have enough
    for (const auto &item : st.vision[0]) {
        if (item == "food") {
            std::cout << "[DEBUG] SurviveStrategy: Found food on current tile during exploration, adding Take food command" << std::endl;
            commands.push_back("Take food");
        }
    }

    // Then collect any minerals that are available
    for (const auto &mineral : minerals) {
        for (const auto &item : st.vision[0]) {
            if (item == mineral) {
                std::cout << "[DEBUG] SurviveStrategy: Found mineral " << mineral << " on current tile during exploration, adding Take command" << std::endl;
                commands.push_back("Take " + mineral);
            }
        }
    }

    if (!commands.empty()) {
        std::cout << "[DEBUG] SurviveStrategy: Adding Inventory command after collection commands" << std::endl;
        commands.push_back("Inventory");
    } else {
        std::cout << "[DEBUG] SurviveStrategy: No resources on current tile, just looking around" << std::endl;
        commands.push_back("Look");
    }

    return commands;
}

bool SurviveStrategy::isMineral(const std::string &item)
{
    return item == "linemate" || item == "deraumere" || item == "sibur" || 
           item == "mendiane" || item == "phiras" || item == "thystame";
} 