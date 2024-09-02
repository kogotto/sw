#include "IO/Commands/CreateMap.hpp"

#include "IO/Events/MapCreated.hpp"
#include "IO/System/EventLog.hpp"

#include "system/Context.hpp"

namespace sw::io {

void processCommand(CreateMap command, Context& context) {
    context.log(
        sw::io::MapCreated{command.width, command.height}
    );
}

} // namespace sw::io
