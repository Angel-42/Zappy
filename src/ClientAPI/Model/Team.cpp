#include "Team.hpp"
#include <algorithm>

namespace clientapi {

//Team::Team() : _name(""), _players(), _eggs() {}
Team::Team(const std::string& name) : _name(name) {}
const std::string& Team::getName() const { return _name; }
const std::vector<int>& Team::getPlayers() const { return _players; }
const std::vector<int>& Team::getEggs() const { return _eggs; }

void Team::addPlayer(int playerId) {
    if (std::find(_players.begin(), _players.end(), playerId) == _players.end())
        _players.push_back(playerId);
}
void Team::removePlayer(int playerId) {
    _players.erase(std::remove(_players.begin(), _players.end(), playerId), _players.end());
}
void Team::addEgg(int eggId) {
    if (std::find(_eggs.begin(), _eggs.end(), eggId) == _eggs.end())
        _eggs.push_back(eggId);
}
void Team::removeEgg(int eggId) {
    _eggs.erase(std::remove(_eggs.begin(), _eggs.end(), eggId), _eggs.end());
}

} // namespace clientapi 