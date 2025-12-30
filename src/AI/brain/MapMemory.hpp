/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** MapMemory.hpp
*/

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>

struct EggInfo
{
    int x, y;
};

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const
    {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

struct MapMemory
{
    std::unordered_map<std::pair<int, int>, std::vector<std::string>, pair_hash> tiles;
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::unordered_map<int, EggInfo> eggs;

    MapMemory() {}

    void setTile(int x, int y, const std::vector<std::string> &contents)
    {
        tiles[{x, y}] = contents;
    }

    const std::vector<std::string> &getTile(int x, int y) const
    {
        static const std::vector<std::string> empty;
        if (tiles.find({x, y}) != tiles.end())
            return tiles.at({x, y});
        return empty;
    }

    void markVisited(int x, int y)
    {
        visited.insert({x, y});
    }

    bool isVisited(int x, int y) const
    {
        return visited.count({x, y}) > 0;
    }

    void rememberEgg(int eggId, int x, int y)
    {
        eggs[eggId] = {x, y};
    }

    void forgetEgg(int eggId)
    {
        eggs.erase(eggId);
    }

    std::vector<std::pair<int, int>> findResource(const std::string &resource) const
    {
        std::vector<std::pair<int, int>> result;
        for (const auto &[pos, contents] : tiles)
            if (std::find(contents.begin(), contents.end(), resource) != contents.end())
                result.push_back(pos);
        return result;
    }

    std::vector<std::pair<int, int>> getUnvisitedTiles() const
    {
        std::vector<std::pair<int, int>> result;
        for (const auto &[pos, _] : tiles)
            if (visited.find(pos) == visited.end())
                result.push_back(pos);
        return result;
    }

    void clear()
    {
        tiles.clear();
        visited.clear();
        eggs.clear();
    }
};
