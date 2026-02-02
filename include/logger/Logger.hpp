#pragma once

#include <fstream>
#include <string>
#include <format>
#include <ctime>

#include "logger/LogLevel.hpp"

namespace logger {

class Logger {
private:
    std::ofstream _logger;
    std::string _path;

public:
    Logger(std::string path);

public:
    void log(const std::string& message, const LogLevel& level = LogLevel::Info);

private:
    void reopen();
};

}
