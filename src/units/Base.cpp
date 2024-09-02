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

void attack(uint32_t attackerId, int damage, Base& target, Context& context) {
    target.receiveDamage(damage);

    context.log(io::UnitAttacked{
        attackerId,
        target._id,
        static_cast<uint32_t>(damage),
        target._hp
    });
    if (target.isDead()) {
        context.log(io::UnitDied{
            target._id
        });
    }
}

bool tryAttackPreviousTarget(Base& unit, uint32_t targetId, Context& context) {
    if (!context.units().contains(targetId)) {
        unit.resetTargetUnitId();
        return false;
    }
    Base& target = asBase(context.units().getById(targetId));
    if (target.isDead()) {
        unit.resetTargetUnitId();
        return false;
    }

    attack(unit._id, unit._strength, target, context);
    return true;
}

using Iter = Units::iterator;

// [min, max)
std::vector<Iter> unitsInRange(Units& units, Cell position, int min, int max) {
    std::vector<Iter> result;

    for (auto it = units.begin(); it != units.end(); ++it) {
        auto& current = asBase(*it);
        const int dist = distance(current.getPosition(), position);
        if (min <= dist && dist < max) {
            result.push_back(it);
        }
    }

    return result;
}

std::optional<Iter> selectMeleeTarget(Cell position, Units& units) {
    auto candidates = unitsInRange(units, position, 1, 2);
    if (candidates.empty()) {
        return std::nullopt;
    }
    const auto it = std::min_element(
        candidates.begin(),
        candidates.end(),
        [] (Iter lhsIt, Iter rhsIt) {
            const Base& lhs = asBase(*lhsIt);
            const Base& rhs = asBase(*rhsIt);
            return std::tie(lhs._hp, lhs._id) < std::tie(rhs._hp, rhs._id);
        }
    );
    if (it == candidates.end()) {
        return std::nullopt;
    }
    return *it;
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
    if (auto targetUnitId = unit.getTargetUnitId(); targetUnitId) {
        return tryAttackPreviousTarget(unit, *targetUnitId, context);
    }
    if (auto targetUnitIt = selectMeleeTarget(unit.getPosition(), context.units()); targetUnitIt) {
        Base& target = asBase(**targetUnitIt);
        unit.setTargetUnitId(target._id);
        attack(unit._id, unit._strength, target, context);
        return true;
    }
    return false;
}

} // namespace sw
