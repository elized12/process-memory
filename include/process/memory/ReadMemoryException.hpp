#pragma once

#include <stdexcept>

namespace process::memory {

class ReadMemoryException : public std::runtime_error {
public:
    ReadMemoryException(const std::string& message) :  std::runtime_error(message) {}
};

}
