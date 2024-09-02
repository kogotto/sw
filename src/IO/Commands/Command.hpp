#pragma once

#include <variant>
#include <iosfwd>

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/Wait.hpp>

namespace sw::io {

using Command = std::variant<
    CreateMap,
    SpawnWarrior,
    SpawnArcher,
    March,
    Wait
>;

inline bool isWaitCommand(const Command& command) {
    return std::holds_alternative<io::Wait>(command);
}

std::ostream& operator<<(std::ostream& stream, const io::Command& command);

} // namespace sw::io
