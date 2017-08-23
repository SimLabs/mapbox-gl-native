#include <mbgl/util/logging.hpp>
#include <mbgl/util/enum.hpp>

#include <fstream>

namespace mbgl {

std::string log_file = "C:\\temp\\mbgl-log.txt";
std::ofstream log_stream(log_file);

void Log::platformRecord(EventSeverity severity, const std::string &msg) {
    log_stream << "[" << Enum<EventSeverity>::toString(severity) << "] " << msg << std::endl;
}

} // namespace mbgl
