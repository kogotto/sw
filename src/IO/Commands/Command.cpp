#include "IO/Commands/Command.hpp"

#include <ostream>

namespace sw::io {

std::ostream& operator<<(std::ostream& stream, const io::Command& command) {
    return stream << "[:|||:]" <<
        std::visit(
            [] (const auto& command) {
                return command.Name;
            },
            command
        );
}

} // namespace sw::io
