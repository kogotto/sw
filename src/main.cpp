#include <iostream>
#include <fstream>

#include <IO/System/CommandParser.hpp>
#include <IO/System/PrintDebug.hpp>

#include <IO/Commands/CreateMap.hpp>
#include <IO/Commands/SpawnWarrior.hpp>
#include <IO/Commands/SpawnArcher.hpp>
#include <IO/Commands/March.hpp>
#include <IO/Commands/Wait.hpp>
#include <IO/Commands/Command.hpp>
#include <IO/Commands/Commands.hpp>
#include <IO/Commands/CommandsStream.hpp>

#include <IO/System/EventLog.hpp>

#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/UnitMoved.hpp>
#include <IO/Events/UnitDied.hpp>
#include <IO/Events/UnitAttacked.hpp>

#include <units/Units.hpp>

namespace sw {

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <typename... Ts>
Overload(Ts...) -> Overload<Ts...>;

void processCreateMap(int tickNumber, io::CreateMap command, sw::EventLog& eventLog) {
    eventLog.log(tickNumber, sw::io::MapCreated{command.width, command.height});
}

void processSpawnWarrior(int tickNumber, io::SpawnWarrior command, sw::Units& units, sw::EventLog& eventLog) {
    sw::io::UnitSpawned data{
        command.unitId,
        "Warrior",
        command.x,
        command.y
    };
    units.spawn(sw::Warrior{command.unitId, command.hp, command.strength});
    eventLog.log(tickNumber, std::move(data));
}

void applyCommand(int tickNumber, const io::Command& command, sw::Units& units, sw::EventLog& eventLog) {
    std::visit(
        Overload{
            [&] (const auto& command) {
                std::cout << "    [" << tickNumber << "] apply command " <<
                    command << std::endl;
            },
            [&] (const io::CreateMap& command) {
                processCreateMap(tickNumber, command, eventLog);
            },
            [&] (const io::SpawnWarrior& command) {
                processSpawnWarrior(tickNumber, command, units, eventLog);
            }
        },
        command
    );
}

bool applyCommandsForThisTick(int tickNumber, io::CommandsStream& stream, sw::Units& units, sw::EventLog& eventLog) {
    for (auto command = stream.fetch(); command; command = stream.fetch()) {
        if (isWaitCommand(*command)) {
            return true;
        }
        applyCommand(tickNumber, *command, units, eventLog);
    }
    return false;
}

bool tick(int tickNumber, io::CommandsStream& stream, sw::Units& units, sw::EventLog& eventLog) {
    return applyCommandsForThisTick(tickNumber, stream, units, eventLog);
}

void mainLoop(const io::Commands& commands) {
    io::CommandsStream stream{commands};
    EventLog eventLog;
    Units units;
    for (int i = 0; ; ++i) {
        if (!tick(i, stream, units, eventLog)) {
            break;
        }
    }
}

} // namespace sw

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2) {
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file) {
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

    const auto commands = io::parseCommands(file);
    mainLoop(commands);

	std::cout << "\n\nEvents:\n";

	EventLog eventLog;

	eventLog.log(1, io::MapCreated{ 10, 10 });
	eventLog.log(1, io::UnitSpawned{ 1, "Archer", 5, 3 });
	eventLog.log(1, io::UnitSpawned{ 2, "Warrior", 5, 3 });
	eventLog.log(2, io::MarchStarted{ 1, 5, 3, 7, 9 });
	eventLog.log(3, io::UnitMoved{ 1, 6, 4 });
	eventLog.log(4, io::UnitAttacked{ 1, 2, 5, 0 });
	eventLog.log(5, io::MarchEnded{ 1, 7, 9 });
	eventLog.log(6, io::UnitDied{ 1 });

	return 0;
}
