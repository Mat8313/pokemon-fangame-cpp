#include "Types.hpp"

namespace Pokemon {
struct XPCurve {
    XPGroup group;

    int xpForLevel(int n) const;

    int xpToNextLevel(int currentLevel) const {
        return xpForLevel(currentLevel + 1) - xpForLevel(currentLevel);
    }
};
} 