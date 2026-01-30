#include "logger/LogLevel.hpp"

namespace logger {

std::string convertToSting(const LogLevel& level) {
    if (level == LogLevel::Debug) {
        return "DEBUG";
    }
    else if (level == LogLevel::Error) {
        return "ERROR";
    }
    else if (level == LogLevel::Fatal) {
        return "FATAL";
    }
    else if (level == LogLevel::Info) {
        return "INFO";
    }
    else if (level == LogLevel::Trace) {
        return "TRACE";
    }

    return "WARN";
}

}
