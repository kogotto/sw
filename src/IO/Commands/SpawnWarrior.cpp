#include "IO/Commands/SpawnWarrior.hpp"

#include "IO/Events/UnitSpawned.hpp"
#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"
#include "units/Units.hpp"

namespace sw::io {

void processCommand(SpawnWarrior command, Context& context) {
    context.units().spawn(
        sw::Warrior{
            command.unitId,
            command.hp,
            command.x,
            command.y,
            command.strength
        }
    );

    context.log(sw::io::UnitSpawned{
        command.unitId,
        "Warrior",
        command.x,
        command.y
    });
}

} // namespace sw::io
