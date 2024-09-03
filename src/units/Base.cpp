#include "units/base.hpp"

#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"

#include "IO/Events/UnitMoved.hpp"
#include "IO/Events/MarchEnded.hpp"
#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"

#include "units/Units.hpp"

namespace sw {

namespace {

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

void attack(Base& unit, Base& target, Context& context) {
    target.receiveDamage(unit._strength);

    context.log(io::UnitAttacked{
        unit._id,
        target._id,
        static_cast<uint32_t>(unit._strength),
        target._hp
    });
    if (target.isDead()) {
        context.log(io::UnitDied{
            target._id
        });
    }
}

bool isRotten(uint32_t id, Units& units) {
    if (!units.contains(id)) {
        return true;
    }
    Base& unit = asBase(units.getById(id));
    if (unit.isDead()) {
        return true;
    }

    return false;
}

std::optional<uint32_t> selectNewMeleeTarget(Cell position, Units& units) {
    auto candidates = units.unitsInRange(position, 1, 2);
    if (candidates.empty()) {
        return std::nullopt;
    }
    const auto it = std::min_element(
        candidates.begin(),
        candidates.end(),
        [] (auto lhsIt, auto rhsIt) {
            const Base& lhs = asBase(*lhsIt);
            const Base& rhs = asBase(*rhsIt);
            return std::tie(lhs._hp, lhs._id) < std::tie(rhs._hp, rhs._id);
        }
    );
    if (it == candidates.end()) {
        return std::nullopt;
    }
    return asBase(**it)._id;
}

std::optional<uint32_t> selectMeleeTarget(Base& unit, Units& units) {
    const auto prevTargetId = unit.getTargetUnitId();
    if (prevTargetId && !isRotten(*prevTargetId, units)) {
        return prevTargetId;
    }
    unit.resetTargetUnitId();
    return selectNewMeleeTarget(unit.getPosition(), units);
}

} // namespace

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
        return true;
    }
    return false;
}

bool processMelee(Base& unit, Context& context) {
    const auto targetId = selectMeleeTarget(unit, context.units());
    if (!targetId) {
        return false;
    }

    Base& target = asBase(context.units().getById(*targetId));
    unit.setTargetUnitId(*targetId);
    attack(unit, target, context);
    return true;
}

} // namespace sw
