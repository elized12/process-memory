#pragma once

#include "process/memory/LinuxMemoryFinder.hpp"
#include "FindValueType.hpp"

namespace process::memory {

class LinuxSmartMemoryFinder {
private:
    process::memory::LinuxMemoryFinder _finder;

public:
    LinuxSmartMemoryFinder(const process::memory::LinuxMemoryFinder& finder);

public:
    std::vector<process::memory::MemoryRecord> findValues(
        int pid,
        const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
        TypeValue targetValue,
        const FindValueType type
    ) const;

};

}
