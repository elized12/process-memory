#pragma once

#include <cstdint>
#include <string>

namespace logger {

enum class LogLevel : std::uint8_t {
    Debug = 0,
    Info = 1,
    Warn = 2,
    Error = 3,
    Fatal = 4,
    Trace = 5
};

std::string convertToSting(const LogLevel& level);

}
