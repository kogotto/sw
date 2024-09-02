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

bool tryAttackPreviousRangeTarget(Archer& unit, uint32_t targetId, Context& context) {
    if (!context.units().contains(targetId)) {
        unit.resetRangeTargetId();
        return false;
    }
    Base& target = asBase(context.units().getById(targetId));
    if (target.isDead()) {
        unit.resetRangeTargetId();
        return false;
    }
    if (auto dist = distance(unit.getPosition(), target.getPosition());
            !inRange(dist, 2, unit._range)) {
        unit.resetRangeTargetId();
        return false;
    }

    rangeAttack(unit, target, context);
    return true;
}

using Iter = Units::iterator;

std::optional<Iter> selectRangeTarget(Archer& unit, Units& units) {
    auto candidates = units.unitsInRange(unit.getPosition(), 2, unit._range + 1);
    if (candidates.empty()) {
        return std::nullopt;
    }
    const auto it = std::min_element(
        candidates.begin(),
        candidates.end(),
        [&unit] (Iter lhsIt, Iter rhsIt) {
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
    return *it;
}

bool processRange(Archer& unit, Context& context) {
    if (auto rangeTargetId = unit.getRangeTargetId(); rangeTargetId) {
        if (tryAttackPreviousRangeTarget(unit, *rangeTargetId, context)) {
            return true;
        }
    }
    if (auto targetIt = selectRangeTarget(unit, context.units()); targetIt) {
        Base& target = asBase(**targetIt);
        unit.setRangeTargetId(target._id);
        rangeAttack(unit, target, context);
        return true;
    }
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
