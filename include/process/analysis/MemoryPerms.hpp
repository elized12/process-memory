#pragma once

#include <cstdint>
#include <string>
#include <format>

#include "process/analysis/ParseException.hpp"

namespace process::analysis {

class MemoryPerms {
private:
    std::uint8_t _perms;

public:
    enum class Perms : std::uint8_t {
        Read = 0b1,
        Write = 0b10,
        Executable = 0b100,
        Private = 0b1000,
        Shared = 0b10000
    };

public:
    explicit MemoryPerms(std::uint8_t perms = 0);

public:
    MemoryPerms& set(Perms perm);
    bool has(Perms perm) const;
    static MemoryPerms parseFromString(const std::string& perm);
    std::string toString() const;
};

}
