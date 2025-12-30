#pragma once
#include <string>
#include "Player.hpp"

namespace clientapi {

/**
 * @brief Represents an egg on the map, which may hatch into a player.
 */
struct Egg {
    int id; ///< Unique egg ID
    Position pos; ///< Position of the egg on the map
    int parentPlayerId; ///< ID of the player who laid the egg
    bool hatched = false; ///< True if the egg has hatched
    bool dead = false; ///< True if the egg is dead
};

} // namespace clientapi 