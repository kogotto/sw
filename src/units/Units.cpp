#include "units/Units.hpp"

#include <algorithm>

namespace sw {

bool isDead(const Unit& unit) {
    return std::visit(
        [] (const auto& unit) {
            return unit.isDead();
        },
        unit
    );
}

void Units::removeDead() {
    units.erase(
        std::remove_if(
            units.begin(),
            units.end(),
            isDead
        ),
        units.end()
    );
}

} // namespace sw
