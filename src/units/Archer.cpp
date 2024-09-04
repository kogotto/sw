#include "units/Archer.hpp"

#include "IO/System/EventLog.hpp"
#include "system/Context.hpp"

#include "IO/Events/UnitAttacked.hpp"
#include "IO/Events/UnitDied.hpp"

#include "units/Units.hpp"

namespace sw {

namespace {

void rangeAttack(Archer& unit, Base& target, Context& context) {
    target.receiveDamage(unit._agility);

    context.log(io::UnitAttacked{
        unit._id,
        target._id,
        static_cast<uint32_t>(unit._agility),
        target._hp
    });
    if (target.isDead()) {
        context.log(io::UnitDied{
            target._id
        });
    }
}

bool inRange(int x, int min, int max) {
    return (min <= x) && (x <= max);
}

bool isRotten(uint32_t id, Archer& archer, Units& units) {
    if (!units.contains(id)) {
        return true;
    }
    Base& unit = asBase(units.getById(id));
    if (unit.isDead()) {
        return true;
    }
    if (auto dist = distance(unit.getPosition(), archer.getPosition());
            !inRange(dist, 2, archer._range)) {
        return true;
    }
    return false;
}

std::optional<uint32_t> selectNewRangeTarget(Archer& unit, Units& units) {
    auto candidates = units.unitsInRange(unit.getPosition(), 2, unit._range + 1);
    if (candidates.empty()) {
        return std::nullopt;
    }
    const auto it = std::min_element(
        candidates.begin(),
        candidates.end(),
        [&unit] (auto lhsIt, auto rhsIt) {
            const Base& lhs = asBase(*lhsIt);
            const int lhsDist = distance(lhs.getPosition(), unit.getPosition());
            const Base& rhs = asBase(*rhsIt);
            const int rhsDist = distance(rhs.getPosition(), unit.getPosition());
            return std::tie(lhsDist, lhs._hp, lhs._id) <
                    std::tie(rhsDist, rhs._hp, rhs._id);
        }
    );
    if (it == candidates.end()) {
        return std::nullopt;
    }
    return asBase(**it)._id;
}

std::optional<uint32_t> selectRangeTarget(Archer& unit, Units& units) {
    const auto prevTargetId = unit.getRangeTargetId();
    if (prevTargetId && !isRotten(*prevTargetId, unit, units)) {
        return prevTargetId;
    }
    unit.resetRangeTargetId();
    return selectNewRangeTarget(unit, units);
}

bool processRange(Archer& unit, Context& context) {
    const auto rangeTargetId = selectRangeTarget(unit, context.units());
    if (!rangeTargetId) {
        return false;
    }
    Base& target = asBase(context.units().getById(*rangeTargetId));
    unit.setRangeTargetId(target._id);
    rangeAttack(unit, target, context);
    return true;
}

} // namespace

bool processUnit(Archer& unit, Context& context) {
    return processRange(unit, context) ||
           processMelee(unit, context) ||
           processMove(unit, context);
}

} // namespace sw
