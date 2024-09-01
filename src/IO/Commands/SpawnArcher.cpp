#include "IO/Commands/SpawnArcher.hpp"

#include "IO/Events/UnitSpawned.hpp"
#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"
#include "units/Archer.hpp"
#include "units/Units.hpp"

namespace sw::io {

void processCommand(SpawnArcher command, Context& context) {
    context.units().spawn(
        sw::Archer{
            command.unitId,
            command.hp,
            command.x,
            command.y,
            command.strength,
            command.range,
            command.agility
    });

    context.log(sw::io::UnitSpawned{
        command.unitId,
        "Archer",
        command.x,
        command.y
    });
}

} // namespace sw::io
