#include "units/Warrior.hpp"

#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"

namespace sw {

namespace {

bool processMelee(Warrior& unit, Context& context) {
    return false;
}

} // namespace

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
