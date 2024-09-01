#include "units/Warrior.hpp"

#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"

#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/MarchEnded.hpp"

namespace sw {

namespace {

bool processMelee(Warrior& unit, Context& context) {
    return false;
}

int sign(int x) {
    if (x < 0) {
        return -1;
    }
    if (x > 0) {
        return 1;
    }
    return 0;
}

Cell getStep(Cell start, Cell finish) {
    return {
        sign(finish.x - start.x),
        sign(finish.y - start.y)
    };
}

bool processMove(Base& unit, Context& context) {
    if (auto target = unit.getTargetPosition(); target) {
        auto step = getStep(unit.getPosition(), *target);
        unit.applyStep(step);

        context.log(io::UnitMoved{
            unit._id,
            unit._x,
            unit._y
        });
        if (unit.getPosition() == *target) {
            unit.resetTargetPosition();
            context.log(io::MarchEnded{
                unit._id,
                unit._x,
                unit._y
            });
        }
    }
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
