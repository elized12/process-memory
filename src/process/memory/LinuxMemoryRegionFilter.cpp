#include "process/memory/LinuxMemoryRegionFilter.hpp"

namespace process::memory {

bool LinuxMemoryRegionFilter::containsGpu(const process::analysis::MemoryBlock& block) const {
    static const std::vector<std::string> GPU_PATTERNS = {
        "i915.gem",
        "i915",
        "amdgpu",
        "radeon",
        "nvidia",
        "nouveau",
        "dma-buf",
        "drm",
        "dri/card",
        "dri/render",
        "dart-heap",
        "dart-zone",
        "wayland",
        "wl_buffer",
        "anon_inode:",
        "gralloc",
        "hwcomposer",
    };

    for (const std::string& pattern : GPU_PATTERNS) {
        if (block.name.contains(pattern)) {
            return true;
        }
    }

    return false;
}

bool LinuxMemoryRegionFilter::isSystemRegion(const process::analysis::MemoryBlock& block) const {
    static const std::vector<std::string> SYSTEM_PATTERNS = {
      "[vvar]"
    };

    for (const std::string& pattern : SYSTEM_PATTERNS) {
        if (block.name == pattern) {
            return true;
        }
    }

    return false;
}

bool LinuxMemoryRegionFilter::shouldSkip(const process::analysis::MemoryBlock& block) const {
    if (!block.perms.has(process::analysis::MemoryPerms::Perms::Read) ||
            !block.perms.has(process::analysis::MemoryPerms::Perms::Write) ||
            block.perms.has(process::analysis::MemoryPerms::Perms::Executable)) {
        return true;
    }

    if (this->containsGpu(block)) {
        return true;
    }

    if (this->isSystemRegion(block)) {
        return true;
    }

    return false;
}

}
