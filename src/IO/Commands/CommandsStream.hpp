#pragma once

#include <optional>

#include "IO/Commands/Commands.hpp"

namespace sw::io {

class CommandsStream {
public:
    CommandsStream(const Commands& commands)
        : commands{commands}
    {}

    std::optional<Command> fetch() {
        if (commands.size() == current) {
          return std::nullopt;
        }
        return commands[current++];
    }
private:
    int current{0};
    const Commands& commands;
};

} // namespace sw::io
