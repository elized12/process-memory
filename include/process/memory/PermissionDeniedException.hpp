#pragma once

#include <stdexcept>

namespace process::memory {

class PermissionDeniedException : public std::runtime_error {
public:
    PermissionDeniedException(const std::string& message) :  std::runtime_error(message) {}
};

}
