/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Planner.cpp - AI decision making and strategy coordination implementation
*/

#include "Planner.hpp"

Planner::Planner() : worldWidth(0), worldHeight(0), lastCommand(""), lastResult("ok"),
            moveFailureCount(0), sameCommandCount(0), lookCounter(0), spiralStep(0)
{
    strategies.emplace_back(std::make_unique<ElevationStrategy>());
    strategies.emplace_back(std::make_unique<SurviveStrategy>());
    // Set BFS navigation callback for SurviveStrategy
    SurviveStrategy::bfsNavigate = [this](const WorldState& st, const std::vector<std::pair<int, int>>& targets) {
        return this->navigateToTarget(st, targets);
    };
}

void Planner::setWorldSize(int width, int height)
{
    worldWidth = width;
    worldHeight = height;
}

void Planner::setLastResult(const std::string &result)
{
    lastResult = result;
    
    if (result == "Elevation underway")
    {
        auto elevationStrategy = dynamic_cast<ElevationStrategy*>(strategies[0].get());
        if (elevationStrategy)
        {
            elevationStrategy->setIncantationResult(result);
        }
    }
}

void Planner::setIncantationSent()
{
    auto elevationStrategy = dynamic_cast<ElevationStrategy*>(strategies[0].get());
    if (elevationStrategy)
    {
        elevationStrategy->setIncantationSent();
    }
}

void Planner::handleBroadcastMessage(const std::string &message)
{
    auto elevationStrategy = dynamic_cast<ElevationStrategy*>(strategies[0].get());
    if (elevationStrategy)
    {
        elevationStrategy->setBroadcastMessage(message);
    }
}

const std::vector<std::unique_ptr<Strategy>>& Planner::getStrategies() const
{
    return strategies;
}

std::vector<std::string> Planner::decide(const WorldState &st)
{
    std::string prevCommand = lastCommand;
    lastCommand = "";

    std::cout << "[DEBUG] Planner: Starting decision process" << std::endl;

    visitedTiles.insert(std::make_pair(st.posX, st.posY));
    updateLocalResourceMap(st);

    if (lastResult == "ko")
    {
        std::cout << "[DEBUG] Planner: Last command failed, handling failure" << std::endl;
        return handleCommandFailure(st, prevCommand);
    }
    else
    {
        moveFailureCount = 0;
    }

    lastResult = "ok";

    for (size_t i = 0; i < strategies.size(); ++i)
    {
        auto &strategy = strategies[i];
        
        std::cout << "[DEBUG] Planner: Checking strategy " << i << " (";
        if (i == 0) std::cout << "ElevationStrategy";
        else if (i == 1) std::cout << "SurviveStrategy";
        else std::cout << "Unknown";
        std::cout << ")" << std::endl;
        
        if (strategy->applicable(st))
        {
            std::cout << "[DEBUG] Planner: Strategy " << i << " is applicable" << std::endl;
            auto commands = strategy->getCommands(st);
            std::cout << "[DEBUG] Planner: Strategy " << i << " returned " << commands.size() << " commands: ";
            for (const auto& cmd : commands) {
                std::cout << cmd << " ";
            }
            std::cout << std::endl;
            
            if (!commands.empty())
            {
                lastCommand = commands[0];
                return commands;
            }
        }
        else
        {
            std::cout << "[DEBUG] Planner: Strategy " << i << " is not applicable" << std::endl;
        }
    }

    std::cout << "[DEBUG] Planner: No strategy returned commands, defaulting to Look" << std::endl;
    lastCommand = "Look";
    return {lastCommand};
}

std::vector<std::string> Planner::handleCommandFailure(const WorldState &st, const std::string &prevCommand)
{
    if (prevCommand == "Take food")
    {
        removeResourceFromLocalMap(st.posX, st.posY, "food");
        return {"Look"};
    }
    else if (prevCommand.find("Take ") == 0)
    {
        std::string resource = prevCommand.substr(5);
        removeResourceFromLocalMap(st.posX, st.posY, resource);
        return {"Look"};
    }
    else if (prevCommand == "Forward")
    {
        moveFailureCount++;
        if (moveFailureCount >= 2)
        {
            moveFailureCount = 0;
            return {"Right", "Look"};
        }
    }
    else if (prevCommand == "Incantation")
    {
        auto elevationStrategy = dynamic_cast<ElevationStrategy*>(strategies[0].get());
        if (elevationStrategy)
        {
            elevationStrategy->setIncantationResult("ko");
        }
        return {"Look"};
    }
    
    return {"Look"};
}

void Planner::updateLocalResourceMap(const WorldState &st)
{
    if (!st.vision.empty())
    {
        resourceMap[std::make_pair(st.posX, st.posY)].clear();

        for (const auto &item : st.vision[0])
        {
            if (item != "player")
            {
                resourceMap[std::make_pair(st.posX, st.posY)].push_back(item);
            }
        }

        int visionIndex = 1;
        for (size_t i = 1; i < st.vision.size(); ++i)
        {
            int relX = 0, relY = 0;

            if (visionIndex < st.vision.size())
            {
                auto resources = st.vision[visionIndex++];
                if (!resources.empty())
                {
                    std::pair<int, int> tilePos = getTilePosition(st, i);
                    resourceMap[tilePos] = resources;
                }
            }
        }
    }
}

std::pair<int, int> Planner::getTilePosition(const WorldState &st, size_t visionIndex)
{
    int relX = 0, relY = 0;
    
    if (visionIndex == 1) { relX = 0; relY = -1; }
    else if (visionIndex == 2) { relX = -1; relY = -1; }
    else if (visionIndex == 3) { relX = 1; relY = -1; }
    else if (visionIndex == 4) { relX = -1; relY = 0; }
    else if (visionIndex == 5) { relX = 1; relY = 0; }
    else if (visionIndex == 6) { relX = 0; relY = 1; }
    else if (visionIndex == 7) { relX = -1; relY = 1; }
    else if (visionIndex == 8) { relX = 1; relY = 1; }

    int tileX = (st.posX + relX + worldWidth) % worldWidth;
    int tileY = (st.posY + relY + worldHeight) % worldHeight;
    
    return {tileX, tileY};
}

void Planner::removeResourceFromLocalMap(int x, int y, const std::string &resource)
{
    auto it = resourceMap.find({x, y});
    if (it != resourceMap.end())
    {
        auto &resources = it->second;
        resources.erase(std::remove(resources.begin(), resources.end(), resource), resources.end());
    }
}

bool Planner::hasFoodOnCurrentTile(const WorldState &st)
{
    if (st.vision.empty() || st.vision[0].empty())
        return false;
    
    return std::find(st.vision[0].begin(), st.vision[0].end(), "food") != st.vision[0].end();
}

int Planner::findResourceInVision(const WorldState &st, const std::string &resource)
{
    for (size_t i = 0; i < st.vision.size(); ++i)
    {
        for (const auto &item : st.vision[i])
        {
            if (item == resource)
            {
                return i;
            }
        }
    }
    return -1;
}

std::vector<std::string> Planner::handleLowFood(const WorldState &st)
{
    if (hasFoodOnCurrentTile(st))
    {
        lastCommand = "Take food";
        return {lastCommand};
    }

    int foodTile = findResourceInVision(st, "food");
    if (foodTile > 0)
    {
        lastCommand = "Forward";
        return {lastCommand};
    }

    auto foodTiles = findResourceInLocalMap("food");
    if (!foodTiles.empty())
    {
        std::string cmd = navigateToTarget(st, foodTiles);
        if (!cmd.empty())
        {
            lastCommand = cmd;
            return {lastCommand};
        }
    }

    lastCommand = "Look";
    return {lastCommand};
}

std::vector<std::pair<int, int>> Planner::findResourceInLocalMap(const std::string &resource)
{
    std::vector<std::pair<int, int>> result;

    for (const auto &entry : resourceMap)
    {
        for (const auto &res : entry.second)
        {
            if (res == resource)
            {
                result.push_back(entry.first);
                break;
            }
        }
    }

    return result;
}

std::vector<std::pair<int, int>> Planner::getUnvisitedTiles()
{
    std::vector<std::pair<int, int>> result;

    std::cout << "[DEBUG] Planner: getUnvisitedTiles - world size: " << worldWidth << "x" << worldHeight << std::endl;
    std::cout << "[DEBUG] Planner: Visited tiles: " << visitedTiles.size() << std::endl;

    for (int x = 0; x < worldWidth; x++)
    {
        for (int y = 0; y < worldHeight; y++)
        {
            std::pair<int, int> pos = {x, y};
            if (visitedTiles.find(pos) == visitedTiles.end())
            {
                result.push_back(pos);
            }
        }
    }
    std::cout << "[DEBUG] Planner: Unvisited tiles: " << result.size() << std::endl;
    return result;
}

std::vector<std::pair<int, int>> Planner::bfsPathToTarget(const WorldState &st, const std::pair<int, int> &target)
{
    std::queue<std::pair<int, int>> q;
    std::unordered_map<int, std::pair<int, int>> parent;
    std::unordered_set<int> visited;
    auto hash = [this](int x, int y) { return y * worldWidth + x; };
    q.push({st.posX, st.posY});
    visited.insert(hash(st.posX, st.posY));
    bool found = false;
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == target.first && y == target.second) { found = true; break; }
        for (auto [dx, dy] : std::vector<std::pair<int, int>>{{0,1},{1,0},{0,-1},{-1,0}}) {
            int nx = (x + dx + worldWidth) % worldWidth;
            int ny = (y + dy + worldHeight) % worldHeight;
            int h = hash(nx, ny);
            if (visited.count(h)) continue;
            visited.insert(h);
            parent[h] = {x, y};
            q.push({nx, ny});
        }
    }
    std::vector<std::pair<int, int>> path;
    if (!found) {
        return path;
    }
    int cx = target.first, cy = target.second;
    while (!(cx == st.posX && cy == st.posY)) {
        path.push_back({cx, cy});
        auto p = parent[hash(cx, cy)];
        cx = p.first; cy = p.second;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::pair<int, int> Planner::findClosestTarget(const WorldState &st, const std::vector<std::pair<int, int>> &targets)
{
    if (targets.empty())
    {
        return {-1, -1};
    }

    int minDist = std::numeric_limits<int>::max();
    std::pair<int, int> closest = {-1, -1};

    for (const auto &target : targets)
    {
        int dx = std::abs(target.first - st.posX);
        int dy = std::abs(target.second - st.posY);

        if (worldWidth > 0)
        {
            dx = std::min(dx, worldWidth - dx);
        }
        if (worldHeight > 0)
        {
            dy = std::min(dy, worldHeight - dy);
        }

        int dist = dx + dy;
        if (dist < minDist)
        {
            minDist = dist;
            closest = target;
        }
    }
    return closest;
}

std::string Planner::navigateToTarget(const WorldState &st, const std::vector<std::pair<int, int>> &targets)
{
    std::vector<std::pair<int, int>> targetList = targets;
    if (targetList.empty()) {
        auto foodTiles = findResourceInLocalMap("food");
        auto unexplored = getUnvisitedTiles();
        if (!foodTiles.empty()) targetList = foodTiles;
        else if (!unexplored.empty()) targetList = unexplored;
        else return "Look";
    }
    
    std::pair<int, int> target = findClosestTarget(st, targetList);
    std::cout << "[DEBUG] Planner: Current position: (" << st.posX << "," << st.posY << ")" << std::endl;
    std::cout << "[DEBUG] Planner: Navigation target: (" << target.first << "," << target.second << ")" << std::endl;
    if (target.first == -1) return "";
    
    auto path = bfsPathToTarget(st, target);
    if (path.empty()) {
        std::cout << "[DEBUG] Planner: No path found to target!" << std::endl;
        return "Look";
    }
    
    auto [nx, ny] = path[0];
    int dx = nx - st.posX;
    int dy = ny - st.posY;
    
    if (worldWidth > 0 && std::abs(dx) > worldWidth / 2)
        dx = (dx > 0) ? dx - worldWidth : dx + worldWidth;
    if (worldHeight > 0 && std::abs(dy) > worldHeight / 2)
        dy = (dy > 0) ? dy - worldHeight : dy + worldHeight;
    
    Orientation targetOrientation;
    if (dx == 1) targetOrientation = Orientation::EAST;
    else if (dx == -1) targetOrientation = Orientation::WEST;
    else if (dy == 1) targetOrientation = Orientation::SOUTH;
    else targetOrientation = Orientation::NORTH;
    
    if (st.orientation != targetOrientation) {
        int currentDir = static_cast<int>(st.orientation);
        int targetDir = static_cast<int>(targetOrientation);
        int diff = (targetDir - currentDir + 4) % 4;
        
        // Add randomness to turning decisions for more organic movement
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0.0, 1.0);
        
        // 20% chance to choose a random direction instead of optimal
        if (dis(gen) < 0.2) {
            if (diff == 2) {
                // For 180-degree turns, randomly choose left or right
                return (dis(gen) < 0.5) ? "Left" : "Right";
            } else if (diff == 1 || diff == 3) {
                // For 90-degree turns, sometimes choose the opposite direction
                if (dis(gen) < 0.1) {
                    return (diff == 1) ? "Left" : "Right";
                }
            }
        }
        
        if (diff == 1) return "Right";
        if (diff == 3) return "Left";
        if (diff == 2) return "Right";
    }
    
    return "Forward";
} 