#pragma once

#include <cstdint>
#include <optional>

#include "units/Cell.hpp"

namespace sw {
class Context;
} // namespace sw

namespace sw {

class Base {
public:
    Base(uint32_t id, int hp, int x, int y, int strength)
        : _id{id}
        , _hp{hp}
        , _x{x}
        , _y{y}
        , _strength{strength}
    {}

    void receiveDamage(int damage) {
        if (damage > _hp) {
            _hp = 0;
        } else {
            _hp -= damage;
        }
    }

    bool isDead() const {
        return _hp == 0;
    }

    Cell getPosition() const {
        return {_x, _y};
    }

    void applyStep(Cell step) {
        _x += step.x;
        _y += step.y;
    }

    std::optional<Cell> getTargetPosition() const {
        return targetPosition;
    }

    void setTargetPosition(int newX, int newY) {
        targetPosition.emplace(newX, newY);
    }

    void resetTargetPosition() {
        targetPosition.reset();
    }

    std::optional<uint32_t> getTargetUnitId() const {
        return targetUnitId;
    }

    void setTargetUnitId(uint32_t newId) {
        targetUnitId.emplace(newId);
    }

    void resetTargetUnitId() {
        targetUnitId.reset();
    }

    uint32_t _id;
    int _hp;

    int _x;
    int _y;

    int _strength;

    std::optional<Cell> targetPosition{};
    std::optional<uint32_t> targetUnitId{};
};

bool processMove(Base& unit, Context& context);
bool processMelee(Base& unit, Context& context);

} // namespace sw
