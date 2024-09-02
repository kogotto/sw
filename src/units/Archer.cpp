#include "units/Archer.hpp"

namespace sw {

namespace {

bool processRange(Archer& unit, Context& context) {
    return false;
}

} // namespace

bool processUnit(Archer& unit, Context& context) {
    if (processRange(unit, context)) {
        return true;
    }
    if (processMelee(unit, context)) {
        return true;
    }
    if (processMove(unit, context)) {
        return true;
    }
    return false;
}

} // namespace sw
