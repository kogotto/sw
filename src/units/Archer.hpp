#pragma once

#include <cstdint>
#include <optional>

#include "units/base.hpp"

namespace sw {
class Context;
} // namespace sw

namespace sw {

class Archer : public Base {
public:
    Archer(uint32_t id, int hp, int x, int y,
            int strength, uint32_t range, uint32_t agility)
        : Base{id, hp, x, y, strength}
        , _range{range}
        , _agility{agility}
    {}

    std::optional<uint32_t> getRangeTargetId() const {
        return rangeTargetId;
    }

    void setRangeTargetId(uint32_t newId) {
        rangeTargetId.emplace(newId);
    }

    void resetRangeTargetId() {
        rangeTargetId.reset();
    }

    uint32_t _range;
    uint32_t _agility;

    std::optional<uint32_t> rangeTargetId{};
};

bool processUnit(Archer& unit, Context& context);

} // namespace sw
