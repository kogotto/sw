#pragma once

#include <cstdint>

namespace sw {
class Context;
} // namespace sw

namespace sw::io
{

struct Wait
{
    constexpr static const char* Name = "WAIT";

    uint32_t ticks {};

    template <typename Visitor>
    void visit(Visitor& visitor)
    {
        visitor.visit("ticks", ticks);
    }
};

void processCommand(Wait command, Context& context);

}
