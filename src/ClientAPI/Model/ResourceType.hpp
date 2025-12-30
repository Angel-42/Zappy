#pragma once
#include <string>

namespace clientapi {

/**
 * @brief Enumerates all resource types in the game.
 */
enum ResourceType {
    FOOD = 0, ///< Food resource
    LINEMATE, ///< Linemate resource
    DERAUMERE, ///< Deraumere resource
    SIBUR, ///< Sibur resource
    MENDIANE, ///< Mendiane resource
    PHIRAS, ///< Phiras resource
    THYSTAME, ///< Thystame resource
    COUNT ///< Number of resource types
};

/**
 * @brief Convert a ResourceType enum value to its string representation.
 * @param type The resource type
 * @return The string name of the resource
 */
inline std::string resourceTypeToString(ResourceType type) {
    switch (type) {
        case FOOD: return "food";
        case LINEMATE: return "linemate";
        case DERAUMERE: return "deraumere";
        case SIBUR: return "sibur";
        case MENDIANE: return "mendiane";
        case PHIRAS: return "phiras";
        case THYSTAME: return "thystame";
        default: return "unknown";
    }
}

} // namespace clientapi 