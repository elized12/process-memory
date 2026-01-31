#pragma once

#include <stdexcept>

namespace process::memory {

class WriteMemoryException : public std::runtime_error {
public:
    WriteMemoryException(const std::string& message) :  std::runtime_error(message) {}
};

}
