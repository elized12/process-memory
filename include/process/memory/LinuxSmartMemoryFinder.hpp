#pragma once

#include "process/memory/LinuxMemoryFinder.hpp"
#include "FindValueType.hpp"

namespace process::memory {

class LinuxSmartMemoryFinder {
private:
    LinuxMemoryFinder _finder;

public:
    LinuxSmartMemoryFinder(const LinuxMemoryFinder& finder);

public:
    std::vector<process::memory::MemoryRecord> findValues(
        int pid,
        const std::vector<process::analysis::MemoryBlock>& memoryBlocks,
        TypeValue targetValue,
        const FindValueType& type
    ) const;

};

}
