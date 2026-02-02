#pragma once

#include <stdexcept>

namespace process::memory {

class ProcessNotExistException : public std::runtime_error {
public:
    ProcessNotExistException(const std::string& message) :  std::runtime_error(message) {}
};

}
