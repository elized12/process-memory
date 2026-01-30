#include "logger/Logger.hpp"

namespace logger {

Logger::Logger(std::string path): _path(std::move(path)) {
    this->reopen();
}

void Logger::log(const std::string& message, const LogLevel& level) {
    this->reopen();
    this->_logger << std::format("{}:{}", convertToSting(level), message) << std::endl;

    this->_logger.flush();
}

void Logger::reopen() {
    if (!this->_logger.is_open()) {
        this->_logger.open(this->_path, std::ios::app);
    }
}

}
