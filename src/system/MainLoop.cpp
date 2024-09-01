#include "system/MainLoop.hpp"

#include "IO/Commands/CommandsStream.hpp"

#include "IO/System/EventLog.hpp"

#include "units/Units.hpp"

#include "system/Context.hpp"

namespace sw {
namespace {

void applyCommand(const io::Command& command, Context& context) {
    std::visit(
        [&] (const auto& command) {
            processCommand(command, context);
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

} // namespace

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
