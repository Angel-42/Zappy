#include "Tile.hpp"
#include <algorithm>

namespace clientapi {

Tile::Tile(int x, int y) : _x(x), _y(y) {}
int Tile::getX() const { return _x; }
int Tile::getY() const { return _y; }
const std::array<int, ResourceType::COUNT>& Tile::getResources() const { return _resources; }
const std::vector<int>& Tile::getPlayers() const { return _players; }
const std::vector<int>& Tile::getEggs() const { return _eggs; }

void Tile::setResource(ResourceType type, int amount) { _resources[type] = amount; }
void Tile::setResources(const std::array<int, ResourceType::COUNT>& resources) { _resources = resources; }
void Tile::addPlayer(int playerId) {
    if (std::find(_players.begin(), _players.end(), playerId) == _players.end())
        _players.push_back(playerId);
}
void Tile::removePlayer(int playerId) {
    _players.erase(std::remove(_players.begin(), _players.end(), playerId), _players.end());
}
void Tile::addEgg(int eggId) {
    if (std::find(_eggs.begin(), _eggs.end(), eggId) == _eggs.end())
        _eggs.push_back(eggId);
}
void Tile::removeEgg(int eggId) {
    _eggs.erase(std::remove(_eggs.begin(), _eggs.end(), eggId), _eggs.end());
}

} // namespace clientapi 