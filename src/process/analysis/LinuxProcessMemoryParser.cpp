#include "process/analysis/LinuxProcessMemoryParser.hpp"
#include <QDebug>

namespace process::analysis {

std::vector<MemoryBlock> LinuxProcessMemoryParser::parseProcess(int pid) const {
    const std::filesystem::path pathMaps = this->ROOT_PROC / std::to_string(pid) / "maps";

    if (!std::filesystem::exists(pathMaps)) {
        throw NotExistsException(std::format("Process with pid = {} not found", pid));
    }

    std::ifstream reader(pathMaps);
    std::vector<std::string> lines;

    std::string line;
    while (std::getline(reader, line)) {
        lines.push_back(std::move(line));
    }

    std::vector<MemoryBlock> memoryBlocks;

    const std::regex regex(R"(^([^-]*)-([^\s]*)\s+([^\s]{4})\s+([^\s]+)\s+([^\s]*)\s(\d+)\s+(.*)$)");
    for (const std::string& line: lines) {
        std::smatch match;
        if (!std::regex_match(line.begin(), line.end(), match, regex)) {
            throw ParseException("Line emtpy in files 'maps'");
        }

        MemoryBlock block;
        block.rangeAddresses = std::pair<address, address>(std::stoull(match[1], nullptr, 16), std::stoull(match[2], nullptr, 16));
        block.perms = MemoryPerms::parseFromString(match[3]);
        block.offset = static_cast<address>(std::stoull(match[4], nullptr, 16));
        block.device = match[5];
        block.inode = match[6];
        block.name = match[7];

        memoryBlocks.push_back(std::move(block));
    }

    return memoryBlocks;
}

const std::filesystem::path LinuxProcessMemoryParser::ROOT_PROC = std::filesystem::path("/proc");

}
