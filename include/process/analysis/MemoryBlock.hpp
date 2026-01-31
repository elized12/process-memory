#pragma once

#include <vector>
#include <cstdint>
#include <string>

#include "process/analysis/MemoryPerms.hpp"

namespace process::analysis {

using address = std::uint64_t;

class MemoryBlock {
public:
    std::pair<address, address> rangeAddresses;
    std::string name;
    std::string inode;
    std::string device;
    MemoryPerms perms;
    address offset;

public:
    MemoryBlock(const std::pair<address, address>& _range = {},
                const std::string& _name = "",
                const MemoryPerms _perms = MemoryPerms(0),
                address _offset = 0,
                const std::string& _inode = "",
                const std::string& _device = "");
};

}
