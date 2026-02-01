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
        !block.perms.has(process::analysis::MemoryPerms::Perms::Write)) {
        return true;
    }

    if (this->containsGpu(block)) {
        return true;
    }

    if (this->isSystemRegion(block)) {
        return true;
    }

    if (this->isLib(block)) {
        return true;
    }

    if (this->isResource(block)) {
        return true;
    }

    return false;
}

bool LinuxMemoryRegionFilter::isLib(const process::analysis::MemoryBlock& block) const {
    if (block.name.empty()) {
        return false;
    }

    static const std::vector<std::regex> LIB_PATTERNS = {
        std::regex(R"(\.so(\.\d+)*$)"),
        std::regex(R"(\.a$)", std::regex::icase),
        std::regex(R"(\.dll$)", std::regex::icase),
        std::regex(R"(\.dylib$)", std::regex::icase),
        std::regex(R"(^\/lib\d*\/)"),
        std::regex(R"(^\/usr\/lib\d*\/)"),
        std::regex(R"(^\/usr\/local\/lib\d*\/)"),
        std::regex(R"(^\/opt\/.*\/lib\d*\/)"),
        std::regex(R"(^\[vdso\])"),
    };

    for (const std::regex& pattern : LIB_PATTERNS) {
            if (std::regex_search(block.name, pattern))
                return true;
    }

    return false;
}

bool LinuxMemoryRegionFilter::isResource(const process::analysis::MemoryBlock& block) const {
    if (block.name.empty()) {
        return false;
    }

    static const std::vector<std::string> RESOURCE_PATTERNS = {
        "/.cache/",
        "/cache/",
        "memfd:",
        "fontconfig",
        "/fonts/",
        ".ttf",
        ".otf",
        ".woff",
        ".woff2",
        ".png",
        ".jpg",
        ".jpeg",
        ".ico",
        ".gif",
        ".svg",
        "/icons/",
        "/themes/",
        "/locale/",
        ".mo",
        ".gresource",
        ".qm",
        "/gvfs/",
        "/gio/modules/",
        "/gtk-",
        "/qt",
        "/mesa_shader_cache",
    };

    for (const auto& pattern : RESOURCE_PATTERNS) {
        if (block.name.find(pattern) != std::string::npos) {
            return true;
        }
    }

    return false;
}

}
