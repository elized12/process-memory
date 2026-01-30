#pragma once

#include <vector>
#include <string>

#include "process/analysis/MemoryBlock.hpp"

namespace process::memory {

class LinuxMemoryRegionFilter {
public:
    bool shouldSkip(const process::analysis::MemoryBlock& block) const;

private:
    bool containsGpu(const process::analysis::MemoryBlock& block) const;
    bool isSystemRegion(const process::analysis::MemoryBlock& block) const;
};

}
