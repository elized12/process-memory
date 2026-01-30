#pragma once

#include <cstdint>

namespace process::memory {

template<class Type>
class MemoryRecord {
public:
    std::uint64_t address;
    Type value;
};

}
