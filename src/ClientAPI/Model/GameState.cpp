#include "GameState.hpp"
#include <utility>

namespace clientapi {

GameState::GameState() : _map(nullptr) {}

Player* GameState::getPlayer(int id) {
    auto it = _players.find(id);
    return it != _players.end() ? &it->second : nullptr;
}
const Player* GameState::getPlayer(int id) const {
    auto it = _players.find(id);
    return it != _players.end() ? &it->second : nullptr;
}
void GameState::addOrUpdatePlayer(const Player& player) {
    _players[player.getId()] = player;
}
void GameState::removePlayer(int id) {
    _players.erase(id);
}

Team* GameState::getTeam(const std::string& name) {
    auto it = _teams.find(name);
    return it != _teams.end() ? &it->second : nullptr;
}
const Team* GameState::getTeam(const std::string& name) const {
    auto it = _teams.find(name);
    return it != _teams.end() ? &it->second : nullptr;
}
void GameState::addOrUpdateTeam(const Team& team) {
    _teams[team.getName()] = team;
}
void GameState::removeTeam(const std::string& name) {
    _teams.erase(name);
}

Egg* GameState::getEgg(int id) {
    auto it = _eggs.find(id);
    return it != _eggs.end() ? &it->second : nullptr;
}
const Egg* GameState::getEgg(int id) const {
    auto it = _eggs.find(id);
    return it != _eggs.end() ? &it->second : nullptr;
}
void GameState::addOrUpdateEgg(const Egg& egg) {
    _eggs[egg.id] = egg;
}
void GameState::removeEgg(int id) {
    _eggs.erase(id);
}

Map* GameState::getMap() { return _map.get(); }
const Map* GameState::getMap() const { return _map.get(); }
void GameState::setMap(const Map& map) { _map = std::make_unique<Map>(map); }

const std::unordered_map<int, Player>& GameState::getPlayers() const { return _players; }
const std::unordered_map<std::string, Team>& GameState::getTeams() const { return _teams; }
const std::unordered_map<int, Egg>& GameState::getEggs() const { return _eggs; }

} // namespace clientapi 