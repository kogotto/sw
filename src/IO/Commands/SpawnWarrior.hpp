#pragma once

#include <cstdint>

namespace sw {
class Context;
}

namespace sw::io
{

struct SpawnWarrior
{
    constexpr static const char* Name = "SPAWN_WARRIOR";

    uint32_t unitId {};
    int x {};
    int y {};
    uint32_t hp {};
    uint32_t strength {};

    template <typename Visitor>
    void visit(Visitor& visitor)
    {
        visitor.visit("unitId", unitId);
        visitor.visit("x", x);
        visitor.visit("y", y);
        visitor.visit("hp", hp);
        visitor.visit("strength", strength);
    }
};

void processCommand(SpawnWarrior command, Context& context);

}
