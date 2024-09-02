#include "IO/Commands/Commands.hpp"

#include <iostream>

#include "IO/System/CommandParser.hpp"

namespace sw::io {

Commands parseCommands(std::istream& stream) {
    Commands result;

    CommandParser parser;
    parser.add<io::CreateMap>(
        [&](auto command)
        {
            result.emplace_back(command);
        }
    ).add<io::SpawnWarrior>(
        [&](auto command)
        {
            result.emplace_back(command);
        }
    ).add<io::SpawnArcher>(
        [&](auto command)
        {
            result.emplace_back(command);
        }
    ).add<io::March>(
        [&](auto command)
        {
            result.emplace_back(command);
        }
    ).add<io::Wait>(
        [&](io::Wait command)
        {
            for (uint32_t i = 0; i < command.ticks + 1; ++i) {
                result.emplace_back(command);
            }
        }
    );
    parser.parse(stream);

    result.emplace_back(io::Wait{0});

    return result;
}

inline void printCommands(const Commands& commands) {
    int tick = 0;
    for (auto&& command : commands) {
        std::cout << "[" << tick << "]" << command << std::endl;
        if (isWaitCommand(command)) {
            ++tick;
        }
    }
}

} // namespace sw::io
