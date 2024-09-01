#include "IO/Commands/March.hpp"

#include "IO/Events/MarchStarted.hpp"
#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"
#include "units/Units.hpp"

namespace sw::io {

void processCommand(io::March command, Context& context) {
    const auto& unit = asBase(context.units().getById(command.unitId));
    // order him to march
    context.log(sw::io::MarchStarted{
        command.unitId,
        unit._x,
        unit._y,
        command.targetX,
        command.targetY
    });
}

} // namespace sw::io
