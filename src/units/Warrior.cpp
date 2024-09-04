#include "units/Warrior.hpp"

namespace sw {

bool processUnit(Warrior& unit, Context& context) {
    return processMelee(unit, context) ||
           processMove(unit, context);
}

} // namespace sw
