#pragma once
#include <vector>
#include <array>
#include "ResourceType.hpp"

namespace clientapi {

/**
 * @brief Represents the requirements for a player to elevate (level up).
 */
struct ElevationRequirement {
    int requiredPlayers; ///< Number of players required for elevation
    std::array<int, ResourceType::COUNT> requiredResources; ///< Resources required for elevation
};

/**
 * @brief Stores and manages elevation requirements for each level.
 */
class ElevationRules {
public:
    /**
     * @brief Default constructor
     */
    ElevationRules() = default;
    /**
     * @brief Set the elevation requirement for a given level
     * @param level The level (1-based)
     * @param req The requirement struct
     */
    void setRequirement(int level, const ElevationRequirement& req);
    /**
     * @brief Get the elevation requirement for a given level
     * @param level The level (1-based)
     * @return Pointer to the requirement struct, or nullptr if not set
     */
    const ElevationRequirement* getRequirement(int level) const;
private:
    std::vector<ElevationRequirement> _requirements; // index = level-1
};

} // namespace clientapi 