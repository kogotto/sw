#pragma once

#include <iosfwd>
#include <cstdint>

namespace sw {
class Context;
}

namespace sw::io {

struct CreateMap {
    constexpr static const char* Name = "CREATE_MAP";

    uint32_t width {};
    uint32_t height {};

    template <typename Visitor>
    void visit(Visitor& visitor)
    {
        visitor.visit("width", width);
        visitor.visit("height", height);
    }
};

void processCommand(CreateMap command, Context& context);

} // namespace sw::io
