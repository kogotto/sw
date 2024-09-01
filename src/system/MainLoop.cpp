#include "system/MainLoop.hpp"

#include "IO/Commands/CommandsStream.hpp"

#include "IO/System/EventLog.hpp"

#include "units/Units.hpp"

#include "system/Context.hpp"

namespace sw {
namespace {

bool applyUnitMove(Unit& unit, Context& context) {
    return std::visit(
        [&] (auto&& unit) {
            return processUnit(unit, context);
        },
        unit
    );
}

bool applyUnitMovesForThisTick(Context& context) {
    bool anotherMoves = false;
    for (auto&& unit : context.units()) {
        anotherMoves = applyUnitMove(unit, context) || anotherMoves;
    }
    return anotherMoves;
}

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
    const bool anotherUnitActions = applyUnitMovesForThisTick(context);
    const bool anotherCommands = applyCommandsForThisTick(stream, context);

    context.units().removeDead();
    return anotherUnitActions || anotherCommands;
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
