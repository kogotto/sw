#pragma once

#include <vector>

#include "IO/Commands/Command.hpp"

namespace sw::io {

using Commands = std::vector<Command>;

Commands parseCommands(std::istream& stream);

void printCommands(const Commands& commands);

} // namespace sw::io
