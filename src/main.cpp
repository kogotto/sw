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

class Context {
public:
    Context(sw::Units& units, sw::EventLog& eventLog)
        : _units{units}
        , _eventLog{eventLog}
    {}

    int getTickNumber() const { return _tickNumber; }
    void setTickNumber(int newNumber) { _tickNumber = newNumber; }

    sw::Units& units() { return _units; }
    const sw::Units& units() const { return _units; }

    sw::EventLog& eventLog() { return _eventLog; }
    const sw::EventLog& eventLog() const { return _eventLog; }

    template <typename TEvent>
    void log(TEvent&& event) {
        eventLog().log(
            getTickNumber(),
            event
        );
    }
private:
    int _tickNumber{0};
    sw::Units& _units;
    sw::EventLog& _eventLog;
};

template <typename... Ts>
struct Overload : Ts... {
    using Ts::operator()...;
};
template <typename... Ts>
Overload(Ts...) -> Overload<Ts...>;

void processCreateMap(io::CreateMap command, Context& context) {
    context.log(
        sw::io::MapCreated{command.width, command.height}
    );
}

void processSpawnWarrior(io::SpawnWarrior command, Context& context) {
    context.units().spawn(
        sw::Warrior{command.unitId, command.hp, command.strength}
    );

    context.log(sw::io::UnitSpawned{
        command.unitId,
        "Warrior",
        command.x,
        command.y
    });
}

void applyCommand(const io::Command& command, Context& context) {
    std::visit(
        Overload{
            [&] (const auto& command) {
                std::cout << "    [" << context.getTickNumber() << "] apply command " <<
                    command << std::endl;
            },
            [&] (const io::CreateMap& command) {
                processCreateMap(command, context);
            },
            [&] (const io::SpawnWarrior& command) {
                processSpawnWarrior(command, context);
            }
        },
        command
    );
}

bool applyCommandsForThisTick(io::CommandsStream& stream, Context& context) {
    for (auto command = stream.fetch(); command; command = stream.fetch()) {
        if (isWaitCommand(*command)) {
            return true;
        }
        applyCommand(*command, context);
    }
    return false;
}

bool tick(io::CommandsStream& stream, Context& context) {
    return applyCommandsForThisTick(stream, context);
}

void mainLoop(const io::Commands& commands) {
    io::CommandsStream stream{commands};
    EventLog eventLog;
    Units units;
    Context context{units, eventLog};
    for (int i = 0; ; ++i) {
        context.setTickNumber(i);
        if (!tick(stream, context)) {
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
