#pragma once

#include <filesystem>
#include <vector>
#include <fstream>
#include <optional>
#include <sstream>
#include <algorithm>

#include "process/ProcessInfo.hpp"
#include "process/detail/FileReadException.hpp"
#include "logger/Logger.hpp"

namespace process::detail {

class LinuxProcessFinder {
private:
    const static std::filesystem::path ROOT_PROC;
    logger::Logger& _logger;

public:
    LinuxProcessFinder(logger::Logger& logger);

public:
    std::vector<process::ProcessInfo> getProcesses() const;
    
private:
    process::ProcessInfo getProccessInfo(const std::filesystem::path& path) const;

};

}
