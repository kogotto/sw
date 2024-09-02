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
    Base(uint32_t id, uint32_t hp, int x, int y)
        : _id{id}
        , _hp{hp}
        , _x{x}
        , _y{y}
    {}

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

    uint32_t _id;
    uint32_t _hp;

    int _x;
    int _y;

    std::optional<Cell> targetPosition{};
};

bool processMove(Base& unit, Context& context);

} // namespace sw
