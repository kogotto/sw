#include "units/Warrior.hpp"

namespace sw {

bool processUnit(Warrior& unit, Context& context) {
    if (processMelee(unit, context)) {
        return true;
    }
    if (processMove(unit, context)) {
        return true;
    }
    return false;
}

} // namespace sw
