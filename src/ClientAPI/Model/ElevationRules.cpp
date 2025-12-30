#include "ElevationRules.hpp"

namespace clientapi {

void ElevationRules::setRequirement(int level, const ElevationRequirement& req) {
    if (_requirements.size() < static_cast<size_t>(level))
        _requirements.resize(level);
    _requirements[level-1] = req;
}

const ElevationRequirement* ElevationRules::getRequirement(int level) const {
    if (level <= 0 || static_cast<size_t>(level) > _requirements.size())
        return nullptr;
    return &_requirements[level-1];
}

} // namespace clientapi 