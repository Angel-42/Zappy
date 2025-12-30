#include "Player.hpp"

namespace clientapi {

Player::Player(int id, const std::string& team, const Position& pos)
    : _id(id), _team(team), _position(pos) {}

//Player::Player() : _id(0), _team(""), _position({0, 0}), _orientation(1), _level(1), _inventory{} {}

int Player::getId() const { return _id; }
const std::string& Player::getTeam() const { return _team; }
const Position& Player::getPosition() const { return _position; }
int Player::getOrientation() const { return _orientation; }
int Player::getLevel() const { return _level; }
const std::array<int, ResourceType::COUNT>& Player::getInventory() const { return _inventory; }

void Player::setPosition(const Position& pos) { _position = pos; }
void Player::setOrientation(int orientation) { _orientation = orientation; }
void Player::setLevel(int level) { _level = level; }
void Player::setInventory(const std::array<int, ResourceType::COUNT>& inv) { _inventory = inv; }
void Player::setResource(ResourceType type, int amount) { _inventory[type] = amount; }

} // namespace clientapi 