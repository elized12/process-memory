#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <format>
#include <regex>
#include <istream>
#include <cerrno>

#include "process/analysis/ParseException.hpp"
#include "process/analysis/NotExistsException.hpp"
#include "process/analysis/MemoryBlock.hpp"

namespace process::analysis {

class LinuxProcessMemoryParser {
private:
    const static std::filesystem::path ROOT_PROC;

public:
    std::vector<MemoryBlock> parseProcess(int pid) const;
};

}
