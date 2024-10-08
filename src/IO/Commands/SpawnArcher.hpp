#pragma once

#include <cstdint>

namespace sw {
class Context;
} // namespace sw

namespace sw::io
{

struct SpawnArcher
{
    constexpr static const char* Name = "SPAWN_ARCHER";

    uint32_t unitId {};
    int x {};
    int y {};
    int hp {};
    int strength {};
    uint32_t range {};
    uint32_t agility {};

    template <typename Visitor>
    void visit(Visitor& visitor)
    {
        visitor.visit("unitId", unitId);
        visitor.visit("x", x);
        visitor.visit("y", y);
        visitor.visit("hp", hp);
        visitor.visit("strength", strength);
        visitor.visit("range", range);
        visitor.visit("agility", agility);

    }
};

void processCommand(SpawnArcher command, Context& context);

}
